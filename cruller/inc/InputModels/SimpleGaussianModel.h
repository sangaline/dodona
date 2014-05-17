#ifndef SimpleGaussianModel_h
#define SimpleGaussianModel_h

#include "InputModels/InputModel.h"

class SimpleGaussianModel : public InputModel {
    double ysigma, xsigma;
    double correlation, correlation_complement;
  public:
    SimpleGaussianModel(double xscale = 0.5, double yscale = 0.5, double correlation = 0);
    InputVector RandomVector(const char* word, Keyboard& k);
    InputVector PerfectVector(const char* word, Keyboard& k);
    double MarginalProbability( InputVector& sigma, const char* word, Keyboard& k);
    double Distance( InputVector& sigma, const char* word, Keyboard& k);
    double VectorDistance(InputVector& vector1, InputVector& vector2);
    void SetXScale(double xscale) { xsigma = xscale*0.5; }
    void SetYScale(double yscale) { ysigma = yscale*0.5; }
    double XScale() { return xsigma*2.0; }
    double YScale() { return ysigma*2.0; }
    void SetCorrelation(double corr);
    void SetScale(double scale) { SetXScale(scale); SetYScale(scale); }
};
#endif
