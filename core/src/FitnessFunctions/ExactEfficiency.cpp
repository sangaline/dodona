#include "FitnessFunctions.h"
#include "InputModels/InputVector.h"

FitnessResult FitnessFunctions::ExactEfficiency(Keyboard& keyboard, InputModel& model, WordList& words) {
    return MonteCarloEfficiency(keyboard, model, words, 0);
}
