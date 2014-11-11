#ifndef NeuralNetworkModel_h
#define NeuralNetworkModel_h

#include "InputModels/SimpleInterpolationModel.h"

struct fann;

class NeuralNetworkModel : public SimpleInterpolationModel {
  private:
    struct fann *ann;
    static unsigned int input_length;

  public:
    NeuralNetworkModel();
    NeuralNetworkModel(const char *filename, unsigned int vector_length = 50, double xscale = 0.5,
            double yscale = 0.5, double correlation = 0, double maxdistance = 0.0, double maxsigmas = 0.0, bool loop = false);
    void SetANN(const char *filename);
    static void CreateInputs(InputVector& v1, InputVector& v2, float* inputs);
    double VectorDistance(InputVector& vector1, InputVector& vector2);
    static unsigned int InputLength() { return input_length; }
};

#endif
