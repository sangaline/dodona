#ifndef FitnessFunctions_h
#define FitnessFunctions_h

#include "InputModels/InputModel.h"
#include "Keyboard.h"
#include "WordList.h"

namespace FitnessFunctions {
    double MonteCarloEfficiency(Keyboard& keyboard, InputModel& model, WordList& words, unsigned int iterations);
};

#endif
