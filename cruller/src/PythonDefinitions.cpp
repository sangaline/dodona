#include "WordList.h"
#include "Polygon.h"
#include "Keyboard.h"

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/list.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/str.hpp>

using namespace boost::python;

//for general use in deep copying
template<typename T> const T DeepCopy(const T& v, dict d) { return T(v); }

/***************** WordList class ***********************/
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(AddWord_overloads, AddWord, 1, 2)
unsigned int (WordList::*Occurances1)(const char*) = &WordList::Occurances;
unsigned int (WordList::*Occurances2)(const unsigned int) = &WordList::Occurances;
/********************************************************/

/***************** Polygon class ************************/
list PolygonVertexList(Polygon& p, bool close_loop) {
    list l;
    for(unsigned int i = 0; i < p.VertexCount(); i++) {
        l.append(make_tuple(p.VertexX(i), p.VertexY(i)));
    }
    if(close_loop && p.VertexCount() > 0) { l.append(make_tuple(p.VertexX(0), p.VertexY(0))); }
    return l;
}
list PolygonVertexListClosed(Polygon& p) {
    return PolygonVertexList(p, true);
}
/********************************************************/

/***************** Keyboard class ************************/
dict KeyboardPolygonDict(Keyboard& k) {
    dict d;
    for(unsigned int c = 0; c < 256; c++) {
        Polygon p( k.GetKey(char(c)) );
        if( p.VertexCount() > 0 ) {
            d[char(c)] = p;
        }
    }
    return d;
}

void AddKeyStr(Keyboard &k, str s, const Polygon& p) {
    char const* c_str = extract<char const*>(s); k.AddKey(c_str[0], p);
}
void RemoveKeyStr(Keyboard &k, str s) {
    char const* c_str = extract<char const*>(s); k.RemoveKey(c_str[0]);
}
Polygon GetKeyStr(Keyboard &k, str s) {
    char const* c_str = extract<char const*>(s); return k.GetKey(c_str[0]);
}

//void AddKey(const unsigned char c, const Polygon& p);
//void RemoveKey(const unsigned char c, const Polygon& p);
/********************************************************/

BOOST_PYTHON_MODULE(cruller)
{

/***************** WordList class ***********************/
    
    class_<WordList>("WordList")
        .def("AddWord", &WordList::AddWord, AddWord_overloads())
        .def("Occurances", Occurances1)
        .def("Occurances", Occurances2)
        .def("TotalOccurances", &WordList::TotalOccurances)
        .def("Word", &WordList::Word)
        .def("Words", &WordList::Words)
        .def("RandomWord", &WordList::RandomWord)
    ;
/********************************************************/

/***************** Polygon class ************************/
    
    class_<Polygon>("Polygon")
        .def("AddVertex", &Polygon::AddVertex)
        .def("ReplaceVertex", &Polygon::ReplaceVertex)
        .def("Reset", &Polygon::Reset)
        .def("VertexList", &PolygonVertexList)
        .def("VertexList", &PolygonVertexListClosed)
        .def("LeftExtreme", &Polygon::LeftExtreme)
        .def("RightExtreme", &Polygon::RightExtreme)
        .def("TopExtreme", &Polygon::TopExtreme)
        .def("BottomExtreme", &Polygon::BottomExtreme)
        .def("__deepcopy__", &DeepCopy<Polygon>)
    ;
/********************************************************/

/***************** Keyboard class ***********************/
    
    class_<Keyboard>("Keyboard")
        .def("AddKey", &AddKeyStr)
        .def("RemoveKey", &RemoveKeyStr)
        .def("GetKey", &GetKeyStr)
        .def("PolygonDict", &KeyboardPolygonDict)
        .def("__deepcopy__", &DeepCopy<Keyboard>)
    ;
/********************************************************/

}
