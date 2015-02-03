#include "InputModels/InputVector.h"

#include "math.h"

#include <vector>
#include <string>
#include <string.h>

using namespace std;

unsigned int InputVector::Length() {
    return xvector.size();
}

unsigned int InputVector::AddPoint(double x, double y, double t) {
    unsigned int i;
    for(i = 0; i < tvector.size(); i++) {
        if(t < tvector.at(i)) {
            break;
        }
    }

    xvector.insert((xvector.begin() + i), x);
    yvector.insert((yvector.begin() + i), y);
    tvector.insert((tvector.begin() + i), t);

    return Length();
}

void InputVector::RemovePoint(int i) {
    while(i<0) { i += Length(); }
    if((unsigned int) i >= xvector.size()) {
        return;
    }
    xvector.erase(xvector.begin() + i);
    yvector.erase(yvector.begin() + i);
    tvector.erase(tvector.begin() + i);
    return;
}

double InputVector::X(int i) {
    while(i<0) { i += Length(); }
    return xvector.at(i);
}

double InputVector::Y(int i) {
    while(i<0) { i += Length(); }
    return yvector.at(i);
}

double InputVector::T(int i) {
    while(i<0) { i += Length(); }
    return tvector.at(i);
}

void InputVector::SetX(int i, double x) {
    while(i<0) { i += Length(); }
    xvector.at(i) = x;
}

void InputVector::SetY(int i, double y) {
    while(i<0) { i += Length(); }
    yvector.at(i) = y;
}

void InputVector::SetT(int i, double t) {
    while(i<0) { i += Length(); }
    tvector.at(i) = t;
}

double InputVector::SpatialLength() {
    double l = 0;
    for(unsigned int i = 1; i < xvector.size(); i++) {
        l += sqrt( pow( xvector.at(i) - xvector.at(i-1), 2) + pow( yvector.at(i) - yvector.at(i-1), 2) );
    }
    return l;
}

double InputVector::TemporalLength() {
    return tvector.at(tvector.size()-1) - tvector.at(0);
}

double InputVector::DeltaPhi(unsigned int i) {
    //change in direction doesn't make sense at the ends
    if( i == 0 || i >= xvector.size() - 1 ) {
        return 0;
    }
    const double oldphi = atan2( (yvector.at(i)-yvector.at(i-1)), (xvector.at(i)-xvector.at(i-1)) );
    const double newphi = atan2( (yvector.at(i+1)-yvector.at(i)), (xvector.at(i+1)-xvector.at(i)) );

    return newphi - oldphi;
}

const char* InputVector::StringForm(Keyboard& k) {
    string s;

    unsigned int lastj = 0;
    for(unsigned int i = 0; i < xvector.size(); i++) {
        if( i>0 && k.GetKey(k.CharN(lastj)).IsInside(xvector.at(i), yvector.at(i)) ) {
            continue;
        }
        for(unsigned int j = 0; j < k.NKeys(); j++) {
            if( k.GetKey(k.CharN(j)).IsInside(xvector.at(i), yvector.at(i)) ) {
                s.push_back(char(k.CharN(j)));
                lastj = j;
                break;
            }
        }
    }

    const char *cstr = s.c_str();
    const unsigned int length = strlen(cstr);
    char *newstring = new char [length+1];
    strcpy(newstring, cstr);

    return newstring;
}
