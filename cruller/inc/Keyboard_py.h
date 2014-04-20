//Functions required for the boost-python interface but not necessary for the C++ compilation
#ifndef Keyboard_py_h
#define Keyboard_py_h

#include "Keyboard.h"

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

dict KeyboardPolygonDict(Keyboard& k) {
    dict d;
    for(unsigned int c = 0; c < 128; c++) {
        Polygon p( k.GetKey(char(c)) );
        if( p.VertexCount() > 0 ) {
            d[char(c)] = p;
        }
    }
    return d;
}

void SetKeyboardPolygonDict(Keyboard& k, dict d) {
    k.Reset();
    for(unsigned int c = 0; c < 128; c++) {
        if(d.has_key(str(char(c)))) {
            Polygon p = extract<Polygon>(d[str(char(c))]);
            k.AddKey(char(c), p);
        }
        else {
            Polygon p;
            k.AddKey(char(c), p);
        }
    }
}

list KeyboardOrderedKeyList(Keyboard& k) {
    list l;
    for(unsigned int c = 0; c < k.NKeys(); c++) {
        Polygon p( k.GetKey(k.CharN(c)) );
        if( p.VertexCount() > 0 ) {
            l.append(char(k.CharN(c)));
        }
    }
    return l;
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


#endif
