#include "Keyboard.h"

#include <boost/random/uniform_int_distribution.hpp>

#include <algorithm>
using namespace std;

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

    idx = k.idx;
    for(unsigned int i = 0; i < idx; i++) {
        entries[i] = k.entries[i];
    }

    generator = k.generator;
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
    bool subtract = false;
    for(unsigned int i = 0; i < idx; i++) {
        if(entries[i] == c) {
            subtract = true;
        }
        if(subtract) {
            entries[i] = entries[i+1];
        }
    }
    //if we found it to remove
    if(subtract) {
        polygons[c] = Polygon();
        idx--;
    }
}

Polygon Keyboard::GetKey(const unsigned char c) const {
    return polygons[c];
}

unsigned char Keyboard::CharN(unsigned int i) {
    return entries[i];
}

int Keyboard::CharIndex(unsigned char c) {
    for(unsigned int i = 0; i < idx; i++) {
        if(entries[i] == c) {
            return i;
        }
    }
    return -1;
}

void Keyboard::Reset() {
    for(unsigned int c = 0; c < 128; c++) {
        polygons[c] = Polygon();
        entries[c] = 0;
    }
    idx = 0;
}

void Keyboard::SwapCharacters(const unsigned char c1, const unsigned char c2) {
    unsigned int i1=0, i2=0;
    unsigned int found = 0;
    for(unsigned int i = 0; i < idx; i++) {
        if(entries[i] == c1) {
            i1=i;
            found++; if(found==2) break;
        }
        if(entries[i] == c2) {
            i2=i;
            found++; if(found==2) break;
        }
    }
    //swap their indexes
    swap(entries[i1], entries[i2]);
    //swap the actual polygons
    swap(polygons[c1], polygons[c2]);
}

//Fisher-yates shuffle
void Keyboard::Randomize() {
    for(unsigned int i = 0; i < idx; i++) {
        boost::random::uniform_int_distribution<> dist(i, idx-1);
        const unsigned int swapidx = dist(generator);
        swap(entries[i], entries[swapidx]);
        swap(polygons[entries[i]], polygons[entries[swapidx]]);
    }
}

void Keyboard::RandomSwap(unsigned int N) {
    boost::random::uniform_int_distribution<> dist(0, idx-1);
    for(unsigned int i = 0; i < N; i++) {
        const unsigned int i1 = dist(generator);
        const unsigned int i2 = dist(generator);
        swap(entries[i1], entries[i2]);
        swap(polygons[entries[i1]], polygons[entries[i2]]);
    }
}

bool Keyboard::operator==(const Keyboard& other) {
    if(idx != other.idx)
        return false;    
    for(unsigned int c = 0; c < idx; c++) {
        if(!(GetKey(entries[c]) == other.GetKey(other.entries[c])))
            return false;
    }
    return true;
}

void Keyboard::RandomNoop() {
    boost::random::uniform_int_distribution<> dist(0, 1);
    dist(generator);
}
