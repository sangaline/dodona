#include "InputModels/InputModel.h"

const char* InputModel::BestMatch(InputVector& vector, Keyboard& keyboard, WordList &words) {
    const char *best_word = 0;
    double best_distance = 0;

    const unsigned int wordlength = vector.Length();
    if(FixedLength() && wordlength <= words.MaxN() && wordlength > 0) {
        for(unsigned int i = 0; i < words.NWords(wordlength); i++) {
            const char *possible_word = words.NWord(wordlength, i);
            const double distance = Distance(vector, possible_word, keyboard);
            if(distance < best_distance || i == 0) {
                best_word = possible_word;
                best_distance = distance;
            }
        }
    }
    else {
        for(unsigned int i = 0; i < words.Words(); i++) {
            const char *possible_word = words.Word(i);
            const double distance = Distance(vector, possible_word, keyboard);
            if(distance < best_distance || i == 0) {
                best_word = possible_word;
                best_distance = distance;
            }
        }
    }

    return best_word;
}
