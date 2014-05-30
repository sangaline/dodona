#include "FitnessFunctions.h"
#include "InputModels/InputVector.h"
#include "RadixTree.h"

#include "string.h"
#include "math.h"
#include <set>
#include <vector>
#include <iostream>
using namespace std;

FitnessResult FitnessFunctions::RadixMonteCarloEfficiency(Keyboard& keyboard, InputModel& model, WordList& words, unsigned int iterations, unsigned int possibility_tries) {
    InputVector *sigma = new InputVector [possibility_tries];

    double efficiency_sum = 0, efficiency_sum2 = 0;
    for(unsigned int iteration = 0; iteration < iterations; iteration++) {
        const char *word = words.RandomWord();

        //construct the set of random vectors and radix tree possibilities
        set<string> possibilities;
        for(unsigned int i = 0; i < possibility_tries; i++) {
            sigma[i] = model.RandomVector(word, keyboard);
            const char * stringform = sigma[i].StringForm(keyboard);
            vector<string> matches = words.GetTree()->Matches(stringform);
            for(vector<string>::iterator it = matches.begin(); it != matches.end(); ++it) {
                possibilities.insert(*it);
            }
        }
        possibilities.insert(word);

        unsigned int matched = 0;
        //go through each of these random vectors
        for(unsigned int sigma_idx = 0; sigma_idx < possibility_tries; sigma_idx++) {
            const char *best_word = 0;
            double best_distance = 0;
            for(set<string>::iterator it = possibilities.begin(); it != possibilities.end(); ++it) {
                const char *possible_word = it->c_str();
                const double distance = model.Distance(sigma[sigma_idx], possible_word, keyboard);
                if(distance < best_distance || best_word == 0) {
                    best_word = possible_word;
                    best_distance = distance;
                }
            }

            if(strcmp(word, best_word) == 0) {
                matched ++;
            }
        }
        const double single_efficiency = double(matched)/double(possibility_tries);
        efficiency_sum += single_efficiency;
        efficiency_sum2 += pow(single_efficiency, 2);

        //why can't I?
        //delete word;
    }
    efficiency_sum /= double(iterations);
    efficiency_sum2 /= double(iterations);
    const double fitness = efficiency_sum;
    const double error = sqrt( (efficiency_sum2 - pow(efficiency_sum, 2))/double(iterations) );

    //why can't I?
    //delete sigma;
    return FitnessResult(iterations, fitness, error);
}
