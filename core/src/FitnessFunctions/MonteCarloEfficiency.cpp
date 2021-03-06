#include "FitnessFunctions.h"
#include "InputModels/InputVector.h"

#include "string.h"
#include "math.h"

FitnessResult FitnessFunctions::MonteCarloEfficiency(Keyboard& keyboard, InputModel& model, WordList& words, unsigned int iterations) {
    bool full_list = false;
    if(iterations == 0) {
        full_list = true;
        iterations = words.Words();
    }

    unsigned int matched = 0, missed = 0;
    for(unsigned int iteration = 0; iteration < iterations; iteration++) {
        const char *word = full_list ? words.Word(iteration) : words.RandomWord();
        InputVector sigma = model.RandomVector(word, keyboard);
        const char *best_word = model.BestMatch(sigma, keyboard, words);

        if(strcmp(word, best_word) == 0) {
            matched += full_list ? words.Occurances(iteration) : 1;
        }
        else {
            missed += full_list ? words.Occurances(iteration) : 1;
        }
    }
    const double fitness = double(matched)/double(missed+matched);

    //no longer right if we're doing the full list - depends on the model, so set to 0 in that case
    const double error = full_list ? 0 : sqrt( fitness*(1.0-fitness)/double(iterations));

    return FitnessResult(iterations, fitness, error);
}
