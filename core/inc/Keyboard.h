#ifndef Keyboard_h
#define Keyboard_h

#include "Polygon.h"

#include <boost/random/mersenne_twister.hpp>

namespace boost {namespace serialization {class access;}}

class Keyboard {
  private:
    Polygon polygons[128];
    unsigned int idx;
    unsigned char entries[128];
    boost::mt19937 generator;
  public:
    Keyboard();
    Keyboard(const Keyboard& k);
    bool operator==(const Keyboard& other);
    void AddKey(const unsigned char c, const Polygon& p);
    void RemoveKey(const unsigned char c);
    Polygon GetKey(const unsigned char c) const;
    unsigned char CharN(unsigned int i);
    int CharIndex(unsigned char c);
    unsigned int NKeys() { return idx; }
    void SwapCharacters(const unsigned char c1, const unsigned char c2);
    void Randomize();
    void RandomSwap(unsigned int N = 1);
    void SetSeed(unsigned int s) { generator.seed(s); }
    void Reset();

  private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& ar, const unsigned int version) {
        ar & idx & polygons & entries;
    }
};

#endif
