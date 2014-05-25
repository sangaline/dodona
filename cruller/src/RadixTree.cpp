#include "RadixTree.h"

#include "string.h"
#include <utility>

using namespace std;

RadixTree::RadixTree(bool firstlast) : firstlast(firstlast) {
    entries = 0;
    for(unsigned int i = 0; i < 256; i++) {
        nodes[i] = 0;
    }
}

RadixTree::~RadixTree() {
    for(unsigned int i = 0; i < 256; i++) {
        if(nodes[i] != 0) {
            delete nodes[i];
        }
    }
}

// make the first and last letters the first two nodes
char* RadixTree::ArrangeWord(const char* word) {
    const unsigned int length = strlen(word);
    char *newword = new char [length+1];
    strcpy(newword, word);
    if(firstlast && length > 2) {
        for(unsigned int i = 2; i < length; i++) {
            newword[i] = word[i-1];
        }
        newword[1] = word[length-1];
    }
    return newword;
}

bool RadixTree::AddWord(const char* word) {
    const int length = strlen(word);
    char *newword = ArrangeWord(word);

    unsigned char index = (unsigned char) word[0];

    bool newnode = false;
    if(nodes[index] == 0) {
        nodes[index] = new RadixTree(false);
        newnode = true;
    }
    if(length > 0) {
        newnode = nodes[index]->AddWord(newword+1) || newnode;
    }

    if(newnode) {
        entries++;
    }

    delete newword;
    return newnode;
}

bool RadixTree::CheckWord(const char* word) {
    char *newword = ArrangeWord(word);
    unsigned char index = (unsigned char) newword[0];
    if(nodes[index] == 0) {
        return false;
    }
    const int length = strlen(word);
    if(length == 0) {
        return true;
    }
    const bool exists = nodes[index]->CheckWord(newword+1);
    delete newword;
    return exists;
}

void PrependLetter(vector<string>& v, const char c) {
    for(unsigned int i = 0; i < v.size(); i++) {
        v[i] = c + v[i];
    }
}
void AppendLetter(vector<string>& v, const char c) {
    for(unsigned int i = 0; i < v.size(); i++) {
        v[i] = v[i] + c;
    }
}

void MergeVectors(vector<string>& v1, vector<string>& v2) {
    v1.reserve(v1.size() + v2.size());
    v1.insert(v1.end(), v2.begin(), v2.end());
}

void StripDuplicates(char *stringform) {
    unsigned int length = strlen(stringform);
    unsigned int unique = 0;
    for(unsigned int i = 0; i < length; i++) {
        stringform[unique] = stringform[i];
        if(stringform[unique] != stringform[i+1]) {
            unique++;
        }
    }
    stringform[unique] = stringform[length];
}

vector<string> RadixTree::Matches(const char* stringform) {
    vector<string> matches;
    unsigned int length = strlen(stringform);

    //deal with the first and last letters if need be
    if(length >= 2 && firstlast) {
        char *newstringform = ArrangeWord(stringform);
        unsigned char index = (unsigned char) newstringform[0];
        unsigned char index2 = (unsigned char) newstringform[1];
        if(nodes[index] != 0 && nodes[index]->nodes[index2] != 0) {
            matches = nodes[index]->nodes[index2]->MatchesHelper(newstringform, false);
            PrependLetter(matches, newstringform[0]);
            AppendLetter(matches, newstringform[1]);
        }
        delete newstringform;
    }
    //otherwise just the first
    else if(length >= 1) {
        unsigned char index = (unsigned char) stringform[0];
        if(nodes[index]) {
            matches = nodes[index]->MatchesHelper(stringform, true);
            PrependLetter(matches, stringform[0]);
        }
    }

    return matches;
}

//first letters are already taken care of, we just need to make sure we're consistent
vector<string> RadixTree::MatchesHelper(const char* stringform, bool termination) {
    vector<string> matches;
    unsigned int length = strlen(stringform);

    for(unsigned int i = 0; i < length; i++) {
        unsigned char index = (unsigned char) stringform[i];
        if(nodes[index] != 0) {
            vector<string> newmatches = nodes[index]->MatchesHelper(stringform+i, termination);
            PrependLetter(newmatches, stringform[i]);
            MergeVectors(matches, newmatches);
        }
    }

    if(length == 1) {
        if(termination && nodes[(unsigned char) stringform[1]] != 0) {
            matches.push_back("");
        }
    }
    if(!termination && nodes[0] != 0) {
        matches.push_back("");
    }

    return matches;
}
