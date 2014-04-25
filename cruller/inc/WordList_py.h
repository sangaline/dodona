//Functions required for the boost-python interface but not necessary for the C++ compilation
#ifndef WordList_py_h
#define WordList_py_h

#include "WordList.h"

//bost include files
#include <boost/unordered_map.hpp>
#include <boost/python/pure_virtual.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>


using namespace boost::python;

typedef boost::unordered_map<std::string, unsigned int> wordmap;


BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(AddWord_overloads, AddWord, 1, 2)
unsigned int (WordList::*Occurances1)(const char*) = &WordList::Occurances;
unsigned int (WordList::*Occurances2)(const unsigned int) = &WordList::Occurances;

dict WordListMapDict(WordList& wl) {
    dict d;
    wordmap wm = wl.GetWordMap();
    for(wordmap::iterator it = wm.begin(); it != wm.end(); it++) {
        d[str(it->first)] = it->second;
    }

    return d;
}

void SetWordListMapDict(WordList& wl, dict d) {
    wl.Reset();
    list keys = d.keys();
    for(unsigned int i = 0; i < len(keys); i++) {
        unsigned int occurances = extract<unsigned int>(d[keys[i]]); 
        std::string key = extract<std::string>(keys[i]);
        wl.AddWord(key.c_str(), occurances);
    }
}


#endif
