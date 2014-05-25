#ifndef RadixTree_h
#define RadixTree_h

#include <vector>
#include <string>

class RadixTree {
  private:
    RadixTree *nodes[256];
    unsigned int entries;
    bool firstlast;
    char* ArrangeWord(const char* word);
    std::vector<std::string> MatchesHelper(const char* stringform, bool termination);
  public:
    RadixTree(bool firstlast = false);
    ~RadixTree();
    //returns false if the word already exists
    bool AddWord(const char* word);
    bool CheckWord(const char* word);
    std::vector<std::string> Matches(const char* stringform);
};

#endif
