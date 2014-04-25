#include "WordList.h"


#include <utility>
using namespace std;

WordList::WordList() {
    Reset();
}

WordList::WordList(const WordList& wl) {
    for(unsigned int i = 0; i < 128; i++) {
        letters[i] = wl.letters[i];
    }

    total_letters = wl.total_letters;
    words = wl.words;
    total = wl.total;

    generator = wl.generator;
    distribution = wl.distribution;
    distribution_current = wl.distribution_current;

    occurance_vector = wl.occurance_vector;
    word_vector = wl.word_vector;
    for(unsigned int i = 0; i < MAXN; i++) {
        Noccurance_vector[i] = wl.Noccurance_vector[i];
        Nword_vector[i] = wl.Nword_vector[i];
    }

}

WordList::WordList(wordmap wm) {
    Reset();
    SetWordMap(wm);
}

WordList::~WordList() {
}

//Should only be used by the constructor since a wordmap is immutable once it is created
void WordList::SetWordMap(wordmap wm) {
    for(wordmap::iterator it = wm.begin(); it != wm.end(); it++) {
        AddWord((it->first).c_str(), it->second); 
    }
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

    //now update the wordmap
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

unsigned int WordList::NWords(const unsigned int N) {
    if(N < 1 || N > MAXN) {
        return 0;
    }
    return Nword_vector[N-1].size();
}

const char* WordList::NWord(const unsigned int N, const unsigned int index) {
    UpdateVectors();
    if(N < 1 || N > MAXN) {
        return 0;
    }
    return Nword_vector[N-1][index].c_str();
}

unsigned int WordList::NOccurances(const unsigned int N, const unsigned int index) {
    UpdateVectors();
    if(N < 1 || N > MAXN) {
        return 0;
    }
    return Noccurance_vector[N-1][index];
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
        for(unsigned int i = 0; i < MAXN; i++) {
            Nword_vector[i].clear();
            Noccurance_vector[i].clear();
        }

        vector< pair<string, unsigned int> > sorted; 
        for(wordmap::iterator it = words.begin(); it != words.end(); it++) {
            sorted.push_back(make_pair(it->first, it->second));
        }
        sort(sorted.begin(), sorted.end(), sortcomparison);

        vector< pair<string, unsigned int> >::iterator it;
        for(it = sorted.begin(); it != sorted.end(); it++) {
            word_vector.push_back(it->first);
            occurance_vector.push_back(it->second);
            const unsigned int l = it->first.length();
            if(l > 0 && l <= MAXN) {
                Nword_vector[l-1].push_back(it->first);
                Noccurance_vector[l-1].push_back(it->second);
            }
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
    distribution_current = true;
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

WordList WordList::operator+(const WordList& other) {    
    wordmap wm1 = GetWordMap();
    wordmap wm2 = other.words;
        
    WordList w = WordList(wm1);
    for(wordmap::iterator it = wm2.begin(); it != wm2.end(); it++) {
        w.AddWord((it->first).c_str(), it->second);
    }
    
    return w;
}

//This could most likely be done better but this is a quick solution
void WordList::Reset() {
    for(unsigned int i = 0; i < 128; i++) {
        letters[i] = 0;
    }
    total_letters = 0;

    vector_current = distribution_current = false;
    total = 0;

    words.clear();
}






