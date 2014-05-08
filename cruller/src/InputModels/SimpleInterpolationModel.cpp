#include "InputModels/SimpleInterpolationModel.h"
#include "InputModels/SimpleGaussianModel.h"
#include "InputModels/Interpolation.h"

#include "Keyboard.h"
#include "Polygon.h"

#include <string.h>
#include <math.h>

SimpleInterpolationModel::SimpleInterpolationModel(unsigned int vector_length, double xscale, double yscale, double maxdistance, double maxsigma, bool loop) {
    maxd = maxdistance;
    maxd2 = maxd*maxd;
    maxs = maxsigma;
    model.SetXScale(xscale);
    model.SetYScale(yscale);
    vlength = vector_length;
    loop_letter = loop;
    Interpolation = &BezierInterpolation;
}

InputVector SimpleInterpolationModel::RandomVector(const char* word, Keyboard& k) {
    InputVector iv = model.RandomVector(word, k);
    if(loop_letter) {
        const unsigned int length = strlen(word);
        for(unsigned int i = 1; i < length; i++) {
            if( word[i] == word[i-1] ) {
                iv.AddPoint(iv.X(i-1), iv.Y(i-1), 1.5*iv.T(i) - 0.5*iv.T(i-1));
            }
        }
    }
    return Interpolation(iv, vlength);
}

InputVector SimpleInterpolationModel::PerfectVector(const char* word, Keyboard& k) {
    InputVector iv = model.PerfectVector(word, k);
    return Interpolation(iv, vlength);
}

double SimpleInterpolationModel::Distance( InputVector& sigma, const char* word, Keyboard& k) { 
    if(maxs > 0) {
        Polygon p = k.GetKey(word[0]);
        const double t = p.TopExtreme();
        const double b = p.BottomExtreme();
        const double r = p.RightExtreme();
        const double l = p.LeftExtreme();

        if( fabs(((t+b) - 2.0*sigma.Y(0))/(t-b)) > maxs || + fabs(((r+l) - 2.0*sigma.X(0))/(r-l)) > maxs) {
            return -1;
        }
    }

    InputVector perfect = model.PerfectVector(word, k);
    perfect = Interpolation(perfect, vlength);
    return VectorDistance(sigma, perfect);
}

double SimpleInterpolationModel::VectorDistance(InputVector& vector1, InputVector& vector2) {
    double d2 = 0;
    for(unsigned int i = 0; i < vlength; i++) {
        d2 += pow( vector1.X(i) - vector2.X(i), 2) + pow( vector1.Y(i) - vector2.Y(i), 2);
        if(d2 > maxd2 && maxd2 > 0) { d2 = maxd2; break; }
    }
    return sqrt(d2);
}

void SimpleInterpolationModel::SetSeed(unsigned int s) {
    generator.seed(s);
    model.SetSeed(s);
}
