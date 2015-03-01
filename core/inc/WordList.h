#ifndef WordList_h
#define WordList_h

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/discrete_distribution.hpp>
#include <boost/unordered_map.hpp>

#include <string>
#include <vector>
#include "boost/serialization/vector.hpp"
#include "boost/serialization/string.hpp"

namespace boost {namespace serialization {class access;}}

#define MAXN 25

class RadixTree;

class WordList {
    typedef boost::unordered_map<std::string, unsigned int> wordmap;

    unsigned int letters[128];
    unsigned int total_letters;
    wordmap words;
    unsigned int total;
    bool letters_current;

    boost::mt19937 generator;
    boost::random::discrete_distribution<> distribution;
    bool distribution_current;

    std::vector<unsigned int> occurance_vector;
    std::vector<std::string> word_vector;
    std::vector<unsigned int> Noccurance_vector[MAXN];
    std::vector<std::string> Nword_vector[MAXN];
    bool vector_current;

    RadixTree *tree;
    bool tree_current;

    void MarkNotCurrent();
    void UpdateLetters();
    void UpdateVectors();
    void UpdateDistribution();
    void UpdateTree();
    void UpdateAll();
  public:
    WordList();
    WordList(const WordList& wl);
    WordList& operator=(const WordList& wl);
    WordList(wordmap wm);
    ~WordList();
    WordList operator+(const WordList& other);

    void SetWordMap(wordmap wm);
    wordmap GetWordMap() { return words; }

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
    int WordIndex(const char* word);
    const char* RandomWord();

    unsigned int TotalLetterOccurances();
    unsigned int LetterOccurances(const char c);
    void SetSeed(unsigned int s) { generator.seed(s); }
    void Reset();

    RadixTree *GetTree() { UpdateTree(); return tree; }

  private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& ar, const unsigned int version) {
        //boost doesn't support serialization of unordered_maps so we'll store the vectors
        UpdateVectors();
        ar & occurance_vector & word_vector;
        //and we'll rebuild the hash map if it doesn't already exist
        if(words.size() == 0) {
            for(unsigned int i = 0; i < word_vector.size(); i++) {
                AddWord(word_vector[i].c_str(), occurance_vector[i]);
            }
            tree_current = false;
        }
    }
};

#endif
