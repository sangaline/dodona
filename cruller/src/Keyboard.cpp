#include "Keyboard.h"

Keyboard::Keyboard() {

}

Keyboard::Keyboard(const Keyboard& k) {
    for(unsigned int c = 0; c < 256; c++) {
        polygons[c] = Polygon(k.GetKey(c));
    }
}

void Keyboard::AddKey(const unsigned char c, const Polygon& p) {
    polygons[c] = Polygon(p);
}

void Keyboard::RemoveKey(const unsigned char c) {
    polygons[c] = Polygon();
}

Polygon Keyboard::GetKey(const unsigned char c) const {
    return polygons[c];
}
