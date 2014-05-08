#ifndef SimpleGaussianModel_h
#define SimpleGaussianModel_h

#include "InputModels/InputModel.h"

class SimpleGaussianModel : public InputModel {
    double ysigma, xsigma;
  public:
    SimpleGaussianModel(double xscale = 0.5, double yscale = 0.5);
    InputVector RandomVector(const char* word, Keyboard& k);
    InputVector PerfectVector(const char* word, Keyboard& k);
    double MarginalProbability( InputVector& sigma, const char* word, Keyboard& k);
    double Distance( InputVector& sigma, const char* word, Keyboard& k);
    double VectorDistance(InputVector& vector1, InputVector& vector2, Keyboard& k);
    void SetXScale(double xscale) { xsigma = xscale*0.5; }
    void SetYScale(double yscale) { ysigma = yscale*0.5; }
    void SetScale(double scale) { SetXScale(scale); SetYScale(scale); }
};
#endif
