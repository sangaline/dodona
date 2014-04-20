//Functions required for the boost-python interface but not necessary for the C++ compilation
#ifndef WordList_py_h
#define WordList_py_h

#include "WordList.h"

//bost include files
#include <boost/python/pure_virtual.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/operators.hpp>

using namespace boost::python;


BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(AddWord_overloads, AddWord, 1, 2)
unsigned int (WordList::*Occurances1)(const char*) = &WordList::Occurances;
unsigned int (WordList::*Occurances2)(const unsigned int) = &WordList::Occurances;

#endif
