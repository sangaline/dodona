#include "InputModels/InputVector.h"

#include "math.h"

unsigned int InputVector::Length() {
    return xvector.size();
}

unsigned int InputVector::AddPoint(double x, double y, double t) {
    unsigned int i;
    for(i = 0; i < tvector.size(); i++) {
        if(t < tvector[i]) {
            break;
        }
    }

    xvector.insert((xvector.begin() + i), x);
    yvector.insert((yvector.begin() + i), y);
    tvector.insert((tvector.begin() + i), t);

    return Length();
}

double InputVector::X(unsigned int i) {
    return xvector[i];
}

double InputVector::Y(unsigned int i) {
    return yvector[i];
}

double InputVector::T(unsigned int i) {
    return tvector[i];
}

double InputVector::SpatialLength() {
    double l = 0;
    for(unsigned int i = 1; i < xvector.size(); i++) {
        l += sqrt( pow( xvector[i] - xvector[i-1], 2) + pow( yvector[i] - yvector[i-1], 2) );
    }
    return l;
}

double InputVector::TemporalLength() {
    return tvector[tvector.size()-1] - tvector[0];
}
