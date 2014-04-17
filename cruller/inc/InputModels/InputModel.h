#ifndef InputModel_h
#define InputModel_h

#include <boost/random/mersenne_twister.hpp>

#include "InputModels/InputVector.h"
#include "Keyboard.h"

class InputModel {
  protected:
    boost::mt19937 generator;

  public:
    virtual InputVector RandomVector(const char* word, Keyboard& k) = 0;
    virtual double Distance(InputVector& vector, const char* word, Keyboard& k) = 0;
};

#endif
