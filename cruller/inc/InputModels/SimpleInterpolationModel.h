#ifndef SimpleInterpolationModel_h
#define SimpleInterpolationModel_h

#include "InputModels/InputModel.h"
#include "InputModels/SimpleGaussianModel.h"

class SimpleInterpolationModel : public InputModel {
    SimpleGaussianModel model, perfectmodel;
    InputVector (*Interpolation)(InputVector&, unsigned int);

    double ysigma, xsigma;
    double maxd, maxd2, maxs;
    unsigned int vlength;
    bool loop_letter;
  public:
    SimpleInterpolationModel(unsigned int vector_length = 50, double xscale = 0.5, double yscale = 0.5, double maxdistance = 0.0, double maxsigmas = 0.0, bool loop = false);
    InputVector RandomVector(const char* word, Keyboard& k);
    double MarginalProbability( InputVector& sigma, const char* word, Keyboard& k);
    double Distance( InputVector& sigma, const char* word, Keyboard& k);
    double VectorDistance(InputVector& vector1, InputVector& vector2, Keyboard& k);
    void SetXScale(double xscale) { model.SetXScale(xscale); }
    void SetYScale(double yscale) { model.SetYScale(yscale); }
    void SetScale(double scale) { SetXScale(scale); SetYScale(scale); }
    void SetMaxDistance(double maxdistance) { maxd = maxdistance; maxd2 = maxd*maxd; }
    void SetVectorLength(unsigned int vector_length) { vlength = vector_length; }
    void SetLoops(bool loop) { loop_letter = loop; }
    void SetInterpolationFunction(InputVector (*fun)(InputVector&, unsigned int)) { Interpolation = fun; }
    void SetSeed(unsigned int s);
};
#endif
