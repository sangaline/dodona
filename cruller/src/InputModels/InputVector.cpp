#include "InputModels/InputVector.h"

unsigned int InputVector::Length() {
    return xvector.size();
}

unsigned int InputVector::AddPoint(double x, double y, double t) {
    xvector.push_back(x);
    yvector.push_back(y);
    tvector.push_back(t);

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
