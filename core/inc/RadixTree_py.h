#ifndef RadixTree_py_h
#define RadixTree_py_h

#include "RadixTree.h"
#include <vector>
#include <string>

#include <boost/python/list.hpp>
#include <boost/python/str.hpp>
using namespace boost::python;

list RadixTreeMatches(RadixTree& tree, const char* stringform) {
    std::vector<std::string> results = tree.Matches(stringform);
    list l;
    for(unsigned int i = 0; i < results.size(); i++) {
        l.append( str(results[i]) );
    }
    return l;
}
#endif
