#include "FitnessFunctions.h"
#include "InputModels/InputVector.h"

#include "string.h"
#include "math.h"

inline double pairwise_efficiency(InputModel& model, InputVector& iv1, InputVector& iv2, const double exp_par) {
    return 1.0-0.5*exp(-exp_par*model.VectorDistance(iv1, iv2));
}

FitnessResult FitnessFunctions::FastEfficiency(Keyboard& keyboard, InputModel& model, WordList& words, double exp_par) {
    //cache the perfect vectors first
    InputVector *perfect = new InputVector [words.Words()];
    for(unsigned int i = 0; i < words.Words(); i++) {
        perfect[i] = model.PerfectVector(words.Word(i), keyboard);
    }

    double totalocc = 0, totaleff = 0;
    //cycle over all pairs of words
    for(unsigned int i = 0; i < words.Words(); i++) {
        double singleeff = 1;
        for(unsigned int j = 0; j < words.Words(); j++) {
            singleeff *= pairwise_efficiency(model, perfect[i], perfect[j], exp_par); //these are being calculate twice, could optimize
        }
        totaleff += singleeff*double(words.Occurances(i));
        totalocc += words.Occurances(i);
    }

    delete [] perfect;
    return FitnessResult(0, totaleff/totalocc, 0);
}
