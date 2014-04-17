#include "InputModels/SimpleGaussianModel.h"
#include "Keyboard.h"
#include "Polygon.h"

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

using namespace std;

SimpleGaussianModel::SimpleGaussianModel(double xscale, double yscale) {
    ysigma = xscale*0.5;
    xsigma = yscale*0.5;
}

InputVector SimpleGaussianModel::RandomVector(const char* word, Keyboard& k) {
    //it's wasteful to remake this every time but I had trouble making it a global member
    boost::normal_distribution<> nd(0.0, 1.0);
    boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > normal(generator, nd);

    InputVector sigma;
    for(unsigned int i = 0; i < strlen(word); i++) {
        Polygon p = k.GetKey(word[i]);
        const double t = p.TopExtreme();
        const double b = p.BottomExtreme();
        const double r = p.RightExtreme();
        const double l = p.LeftExtreme();

        const double x = normal()*xsigma*(r-l) + 0.5*(r+l);
        const double y = normal()*ysigma*(t-b) + 0.5*(t+b);
        sigma.AddPoint(x, y, double(i));
    }
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
