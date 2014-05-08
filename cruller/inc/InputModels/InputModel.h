#ifndef InputModel_h
#define InputModel_h

#include <boost/random/mersenne_twister.hpp>

#include "InputModels/InputVector.h"
#include "Keyboard.h"

class InputModel {
  protected:
    boost::mt19937 generator;
    bool fixed_length;

  public:
    InputModel() { fixed_length = false; }
    bool FixedLength() { return fixed_length; }

    virtual InputVector RandomVector(const char* word, Keyboard& k) = 0;
    virtual InputVector PerfectVector(const char* word, Keyboard& k) = 0;
    virtual double Distance(InputVector& vector, const char* word, Keyboard& k) = 0;
    virtual double VectorDistance(InputVector& vector1, InputVector& vector2) = 0;
    virtual void SetSeed(unsigned int s) { generator.seed(s); }
};

#endif
