#ifndef Keyboard_h
#define Keyboard_h

#include "Polygon.h"

class Keyboard {
  private:
    Polygon polygons[256];
  public:
    Keyboard();
    Keyboard(const Keyboard& k);
    void AddKey(const unsigned char c, const Polygon& p);
    void RemoveKey(const unsigned char c);
    Polygon GetKey(const unsigned char c) const;
};

#endif
