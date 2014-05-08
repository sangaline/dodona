//Functions required for the boost-python interface but not necessary for the C++ compilation
#ifndef InputModels_py_h
#define InputModels_py_h

#include "InputModels/InputModel.h"

//Boost include files
#include <boost/python/class.hpp>
#include <boost/python/wrapper.hpp>
#include <boost/python/pure_virtual.hpp>
#include <boost/python/list.hpp>

using namespace boost::python;


/***************** InputVector class ********************/
list InputVectorList(InputVector& sigma) {
    list l;
    for(unsigned int i = 0; i < sigma.Length(); i++) {
        l.append(make_tuple(sigma.X(i), sigma.Y(i), sigma.T(i)));
    }
    return l;
}
/********************************************************/

/***************** InputModel wrappers ******************/
class InputModelWrapper : public InputModel, public boost::python::wrapper<InputModel> {
    double Distance(InputVector& vector, const char* word, Keyboard& k) {
        return this->get_override("Distance")(vector, word, k);
    }
    double VectorDistance(InputVector& vector1, InputVector& vector2, Keyboard& k) {
        return this->get_override("VectorDistance")(vector1, vector2, k);
    }
    InputVector RandomVector(const char* word, Keyboard& k) {
        return this->get_override("RandomVector")(word, k);
    }
    InputVector PerfectVector(const char* word, Keyboard& k) {
        return this->get_override("PerfectVector")(word, k);
    }
};
/********************************************************/

#endif
