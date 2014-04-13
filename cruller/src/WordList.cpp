#include "WordList.h"


#include <utility>
using namespace std;

WordList::WordList() {
    for(unsigned int i = 0; i < 128; i++) {
        letters[i] = 0;
    }
    total_letters = 0;

    vector_current = distribution_current = false;
    total = 0;
}

WordList::~WordList() {
}

unsigned int WordList::AddWord(const char *word, const unsigned int occurances) {
    string wordstring(word);

    //first update the letter occurances:
    for(unsigned int i = 0; i < wordstring.length(); i++) {
        unsigned char c = (unsigned char) wordstring[i];
        if(c < 128) {
            letters[c] += occurances;
            total_letters += occurances;
        }
    }

    //no update the wordmap
    total += occurances;
    vector_current = distribution_current = false;
    const wordmap::iterator it = words.find(wordstring);
    if(it != words.end()) {
        it->second += occurances;
        return it->second;
    }
    words.insert(make_pair(wordstring, occurances));
    return occurances;
}

unsigned int WordList::Occurances(const char *word) {
    const wordmap::iterator it = words.find(string(word));
    if(it != words.end()) {
        return it->second;
    }
    return 0;
}

unsigned int WordList::Words() {
    return words.size();
}

const char* WordList::Word(const unsigned int index) {
    UpdateVectors();
    return word_vector[index].c_str();
}

unsigned int WordList::Occurances(const unsigned int index) {
    UpdateVectors();
    return occurance_vector[index];
}

const char* WordList::RandomWord() {
    UpdateAll();
    return word_vector[distribution(generator)].c_str();
}

bool sortcomparison(const pair<string, unsigned int> &a, const pair<string, unsigned int> &b ) { 
    return (a.second > b.second); 
}

void WordList::UpdateVectors() {
    if(!vector_current) {
        word_vector.clear();
        occurance_vector.clear();

        vector< pair<string, unsigned int> > sorted; 
        for(wordmap::iterator it = words.begin(); it != words.end(); it++) {
            sorted.push_back(make_pair(it->first, it->second));
        }
        sort(sorted.begin(), sorted.end(), sortcomparison);

        vector< pair<string, unsigned int> >::iterator it;
        for(it = sorted.begin(); it != sorted.end(); it++) {
            word_vector.push_back(it->first);
            occurance_vector.push_back(it->second);
        }
        sorted.clear();

        vector_current = true;
    }
    distribution_current = false;
}

void WordList::UpdateDistribution() {
    if(!distribution_current) {
        distribution = boost::random::discrete_distribution<>(occurance_vector);
    }
}

void WordList::UpdateAll() {
    UpdateVectors();
    UpdateDistribution();
}

unsigned int WordList::LetterOccurances(const char c) {
    unsigned char usc = (unsigned char) c;
    if(usc < 128) {
        return letters[usc];
    }
    return 0;
}
