#include "InputModels/SimpleGaussianModel.h"
#include "Keyboard.h"
#include "Polygon.h"

#include "math.h"

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

using namespace std;

SimpleGaussianModel::SimpleGaussianModel(double xscale, double yscale, double corr) {
    ysigma = xscale*0.5;
    xsigma = yscale*0.5;
    fixed_length = true;
    SetCorrelation(corr);
}

void SimpleGaussianModel::SetCorrelation(double corr) {
    if(corr < 0) corr = 0;
    if(corr > 1) corr = 1;
    correlation = corr;
    correlation_complement = sqrt(1.0-correlation*correlation);
}

InputVector SimpleGaussianModel::RandomVector(const char* word, Keyboard& k) {
    //it's wasteful to remake this every time but I had trouble making it a global member
    boost::normal_distribution<> nd(0.0, 1.0);
    boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > normal(generator, nd);

    InputVector sigma;
    double lastx = normal(), lasty = normal();
    for(unsigned int i = 0; i < strlen(word); i++) {
        Polygon p = k.GetKey(word[i]);
        const double t = p.TopExtreme();
        const double b = p.BottomExtreme();
        const double r = p.RightExtreme();
        const double l = p.LeftExtreme();

        if(i > 0) {
            lastx = lastx*correlation + normal()*correlation_complement;
            lasty = lasty*correlation + normal()*correlation_complement;
        }

        const double x = lastx*xsigma*(r-l) + 0.5*(r+l);
        const double y = lasty*ysigma*(t-b) + 0.5*(t+b);
        sigma.AddPoint(x, y, double(i));
    }
    return sigma;
}

InputVector SimpleGaussianModel::PerfectVector(const char* word, Keyboard& k) {
    const double tmp_xsigma = xsigma; xsigma = 0;
    const double tmp_ysigma = ysigma; ysigma = 0;
    InputVector sigma = RandomVector(word, k);
    xsigma = tmp_xsigma;
    ysigma = tmp_ysigma;
    return sigma;
}

double SimpleGaussianModel::MarginalProbability( InputVector& sigma, const char* word, Keyboard& k) {
    if(sigma.Length() != strlen(word)) {
        return 0;
    }

    double probability = 1;
    for(unsigned int i = 0; i < sigma.Length(); i++) {
        Polygon p = k.GetKey(word[i]);
        const double t = p.TopExtreme();
        const double b = p.BottomExtreme();
        const double r = p.RightExtreme();
        const double l = p.LeftExtreme();

        const double x = sigma.X(i);
        const double y = sigma.Y(i);
        const double xmu = 0.5*(r+l);
        const double ymu = 0.5*(t+b);
        const double xsd = xsigma*(r-l);
        const double ysd = ysigma*(t-b);

        probability *= (0.15915494309/(xsd*ysd))*exp(-0.5*(pow((x-xmu)/xsd,2)+pow((y-ymu)/ysd,2)));
    }

    return probability;
}

double SimpleGaussianModel::Distance( InputVector& sigma, const char* word, Keyboard& k) { 
    if(sigma.Length() != strlen(word)) {
        return 1;
    }

    double probability = 1;
    for(unsigned int i = 0; i < sigma.Length(); i++) {
        Polygon p = k.GetKey(word[i]);
        const double t = p.TopExtreme();
        const double b = p.BottomExtreme();
        const double r = p.RightExtreme();
        const double l = p.LeftExtreme();

        const double xsd = xsigma*(r-l);
        const double ysd = ysigma*(t-b);

        probability *= (0.15915494309/(xsd*ysd));
    }

    return (probability - MarginalProbability(sigma, word, k))/probability;
}

double SimpleGaussianModel::VectorDistance(InputVector& vector1, InputVector& vector2) {
    double d2 = 0;
    for(unsigned int i = 0; i < vector1.Length(); i++) {
        d2 += pow( vector1.X(i) - vector2.X(i), 2) + pow( vector1.Y(i) - vector2.Y(i), 2);
    }
    return sqrt(d2);
}
