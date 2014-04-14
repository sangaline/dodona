#include "Keyboard.h"

Keyboard::Keyboard() {
    idx = 0;
    for(unsigned int c = 0; c < 128; c++) {
        polygons[c] = Polygon();
    }
}

Keyboard::Keyboard(const Keyboard& k) {
    for(unsigned int c = 0; c < 128; c++) {
        polygons[c] = Polygon(k.GetKey(c));
    }
}

void Keyboard::AddKey(const unsigned char c, const Polygon& p) {
    if(polygons[c].VertexCount() > 0) {
        RemoveKey(c);
    }
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
            entries[i] = entries[i+1];
        }
    }
    idx--;
    polygons[idx] = Polygon();
}

Polygon Keyboard::GetKey(const unsigned char c) const {
    return polygons[c];
}

unsigned char Keyboard::CharN(unsigned int i) {
    return entries[i];
}

void Keyboard::Reset() {
    for(unsigned int c = 0; c < 128; c++) {
        polygons[c] = Polygon();
        entries[c] = 0;
    }
    idx = 0;
}
