#ifndef WordList_h
#define WordList_h

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/discrete_distribution.hpp>
#include <boost/unordered_map.hpp>

#include <string>
#include <vector>

#define MAXN 25

class WordList {
    typedef boost::unordered_map<std::string, unsigned int> wordmap;

    unsigned int letters[128];
    unsigned int total_letters;
    wordmap words;
    unsigned int total;

    boost::mt19937 generator;
    boost::random::discrete_distribution<> distribution;
    bool distribution_current;

    std::vector<unsigned int> occurance_vector;
    std::vector<std::string> word_vector;
    std::vector<unsigned int> Noccurance_vector[MAXN];
    std::vector<std::string> Nword_vector[MAXN];
    bool vector_current;

    void UpdateVectors();
    void UpdateDistribution();
    void UpdateAll();
  public:
    WordList();
    WordList(wordmap wm);
    ~WordList();
//    WordList operator+(const WordList& other);

    void SetWordMap(wordmap wm);

    unsigned int AddWord(const char *word, const unsigned int occurances = 1);
    unsigned int Occurances(const char *word);
    unsigned int TotalOccurances() { return total; }
    const char* Word(const unsigned int index);
    unsigned int Occurances(const unsigned int index);
    unsigned int Words();
    const char* NWord(const unsigned int N, const unsigned int index);
    unsigned int NOccurances(const unsigned int N, const unsigned int index);
    unsigned int NWords(const unsigned int N);
    unsigned int MaxN() { return MAXN; }
    const char* RandomWord();

    unsigned int TotalLetterOccurances() { return total_letters; }
    unsigned int LetterOccurances(const char c);
};

#endif
