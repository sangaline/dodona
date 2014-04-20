//Functions required for the boost-python interface but not necessary for the C++ compilation
#ifndef Polygon_py_h
#define Polygon_py_h

#include "Polygon.h"

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/wrapper.hpp>
#include <boost/python/pure_virtual.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/list.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/str.hpp>

using namespace boost::python;

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

#endif
