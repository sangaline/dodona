#include "InputModels/SimpleInterpolationModel.h"
#include "InputModels/SimpleGaussianModel.h"
#include "InputModels/Interpolation.h"

#include "Keyboard.h"
#include "Polygon.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

namespace {
    void HandleDoubleLetters(InputVector &iv, const char* word, Keyboard& k, bool loop_letter) {
        const unsigned int length = strlen(word);
        unsigned int doubles = 0;
        for(unsigned int i = 1; i < length + doubles; i++) {
            if( word[i] == word[i-1] ) {
                if(loop_letter) {
                    Polygon p = k.GetKey(word[i]);
                    const double delta_x = 0.5*(p.RightExtreme() - p.LeftExtreme());
                    const double delta_y = 0.5*(p.TopExtreme() - p.BottomExtreme());
                    iv.AddPoint(iv.X(doubles+i-1) + delta_x, iv.Y(doubles+i-1) + delta_y, 0.5*(iv.T(doubles+i) + iv.T(doubles+i-1)));
                    doubles++;
                }
                else { 
                    if(i >= length) break;
                    int iter = i-doubles;
                    iv.RemovePoint(iter);
                    doubles++;
                }
            }
        }
    }
};

SimpleInterpolationModel::SimpleInterpolationModel(unsigned int vector_length, double xscale, double yscale, double correlation, double maxdistance, double maxsigma, bool loop) {
    maxd = maxdistance;
    maxd2 = maxd*maxd;
    maxs = maxsigma;
    model.SetXScale(xscale);
    model.SetYScale(yscale);
    vlength = vector_length;
    loop_letter = loop;
    interpolation = &SpatialInterpolation;
    SetCorrelation(correlation);
}

InputVector SimpleInterpolationModel::RandomVector(const char* word, Keyboard& k) {
    InputVector iv = model.RandomVector(word, k);
    HandleDoubleLetters(iv, word, k, loop_letter);
    return Interpolation(iv, vlength);
}

InputVector SimpleInterpolationModel::PerfectVector(const char* word, Keyboard& k) {
    InputVector iv = model.PerfectVector(word, k);
    HandleDoubleLetters(iv, word, k, loop_letter);
    return Interpolation(iv, vlength);
}

double SimpleInterpolationModel::Distance( InputVector& sigma, const char* word, Keyboard& k) { 
    if(maxs > 0) {
        Polygon p = k.GetKey(word[0]);
        const double t = p.TopExtreme();
        const double b = p.BottomExtreme();
        const double r = p.RightExtreme();
        const double l = p.LeftExtreme();

        if( pow(((t+b) - 2.0*sigma.Y(0))/(model.YScale()*(t-b)), 2) + pow(((r+l) - 2.0*sigma.X(0))/(model.XScale()*(r-l)), 2) > maxs*maxs ) {
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

InputVector SimpleInterpolationModel::Interpolation(InputVector& iv, unsigned int N) const {
    return interpolation(iv, N);
}
