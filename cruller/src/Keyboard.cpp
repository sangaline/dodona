#include "Keyboard.h"

Keyboard::Keyboard() {
    idx = 0;
}

Keyboard::Keyboard(const Keyboard& k) {
    for(unsigned int c = 0; c < 128; c++) {
        polygons[c] = Polygon(k.GetKey(c));
    }
}

void Keyboard::AddKey(const unsigned char c, const Polygon& p) {
    polygons[c] = Polygon(p);
    entries[idx] = c;
    idx++;
}

void Keyboard::RemoveKey(const unsigned char c) {
    polygons[c] = Polygon();
    bool subtract = false;
    for(unsigned int i = 0; i < 127; i++) {
        if(entries[i] == c) {
            subtract = true;
        }
        if(subtract) {
            polygons[i] = polygons[i+1];
        }
    }
    polygons[127] = Polygon();
    idx--;
}

Polygon Keyboard::GetKey(const unsigned char c) const {
    return polygons[c];
}

unsigned char Keyboard::CharN(unsigned int i) {
    return entries[idx];
}
