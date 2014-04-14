#ifndef Keyboard_h
#define Keyboard_h

#include "Polygon.h"

class Keyboard {
  private:
    Polygon polygons[128];
    unsigned int idx;
    unsigned char entries[128];
  public:
    Keyboard();
    Keyboard(const Keyboard& k);
    void AddKey(const unsigned char c, const Polygon& p);
    void RemoveKey(const unsigned char c);
    Polygon GetKey(const unsigned char c) const;
    unsigned char CharN(unsigned int i);
    void Reset();
};

#endif
