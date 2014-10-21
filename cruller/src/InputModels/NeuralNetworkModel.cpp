#include "InputModels/NeuralNetworkModel.h"

#include "InputModels/InputVector.h"

#include "fann.h"

#include <algorithm>
#include "math.h"
using namespace std;

NeuralNetworkModel::NeuralNetworkModel(const char *filename, unsigned int vector_length, double xscale, double yscale, double correlation, double maxdistance, double maxsigmas, bool loop) :
    SimpleInterpolationModel(vector_length, xscale, yscale, correlation, maxdistance, maxsigmas, loop) {

    ann = fann_create_from_file(filename);
}

// ==  The meaning of each entry in inputs is as follows:
//  0 - length difference, 
//  1 - x distance2
//  2 - y distance2
//  3 - dx distance2
//  4 - dy distance2
//  5 - ddx distance2
//  6 - ddy distance2
//  7 - x distance2 of first point
//  8 - y distance2 of first point
//  9 - x distance2 of last point
// 10 - y distance2 of last point
// IMPORTANT: If the number of inputs changes then make sure you change this next line!
unsigned int NeuralNetworkModel::input_length = 11;
void NeuralNetworkModel::CreateInputs(InputVector& v1, InputVector& v2, float* inputs) {
    const unsigned int vlength = v1.Length();

    //these hold onto the previous derivatives to calculate the second derivatives each time
    double lastdx1 = v1.X(1) - v1.X(0);
    double lastdx2 = v2.X(1) - v2.X(0);
    double lastdy1 = v1.Y(1) - v1.Y(0);
    double lastdy2 = v2.Y(1) - v2.Y(0);
    //these will accumulate the lengths squared
    double length1 = sqrt(lastdx1*lastdx1 + lastdy1*lastdy1);
    double length2 = sqrt(lastdx2*lastdx2 + lastdy2*lastdy2);
    //the rest we'll accumulate in place
    inputs[1] = pow(v2.X(0)-v1.X(0), 2) + pow(v2.X(1)-v1.X(1), 2);
    inputs[2] = pow(v2.Y(0)-v1.Y(0), 2) + pow(v2.Y(1)-v1.Y(1), 2);
    inputs[3] = pow(lastdx2 - lastdx1, 2);
    inputs[4] = pow(lastdy2 - lastdy1, 2);
    inputs[5] = 0;
    inputs[6] = 0;
    for(unsigned int i = 2; i < vlength; i++) {
        const double newdx1 = v1.X(i) - v1.X(i-1);
        const double newdx2 = v2.X(i) - v2.X(i-1);
        const double newdy1 = v1.Y(i) - v1.Y(i-1);
        const double newdy2 = v2.Y(i) - v2.Y(i-1);

        //accumulate lengths
        length1 += sqrt(newdx1*newdx1 + newdy1*newdy1);
        length2 += sqrt(newdx2*newdx2 + newdy2*newdy2);
        //and the others
        inputs[1] += pow(v2.X(i)-v1.X(i), 2);
        inputs[2] += pow(v2.Y(i)-v1.Y(i), 2);
        inputs[3] += pow(newdx2 - newdx1, 2);
        inputs[4] += pow(newdy2 - newdy1, 2);
        inputs[5] += pow( (newdx2-lastdx2) - (newdx1-lastdx1), 2);
        inputs[6] += pow( (newdy2-lastdy2) - (newdy1-lastdy1), 2);

        lastdx1 = newdx1;
        lastdx2 = newdx2;
        lastdy1 = newdy1;
        lastdy2 = newdy2;
    }
    //the length requires scaling
    inputs[0] = fabs(length2 - length1);
    if( length1 > 0 || length2 > 0 ) { inputs[0] /= max(length1, length2); }

    //2-6 are already complete, these can be computed in one shot
    inputs[7] = pow(v2.X(0) - v1.X(0), 2);
    inputs[8] = pow(v2.Y(0) - v1.Y(0), 2);
    inputs[9] = pow(v2.X(-1) - v1.X(-1), 2);
    inputs[10] = pow(v2.Y(-1) - v1.Y(-1), 2);
}

double NeuralNetworkModel::VectorDistance(InputVector& vector1, InputVector& vector2) {
    float inputs[11];
    CreateInputs(vector1, vector2, inputs);
    return 1.0 - double(*fann_run(ann, inputs));
}
