#include "WordList.h"
#include "Polygon.h"

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/list.hpp>
#include <boost/python/tuple.hpp>

using namespace boost::python;

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
    if(close_loop) { l.append(make_tuple(p.VertexX(0), p.VertexY(0))); }
    return l;
}
list PolygonVertexListClosed(Polygon& p) {
    return PolygonVertexList(p, true);
}
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
    ;
/********************************************************/

}

