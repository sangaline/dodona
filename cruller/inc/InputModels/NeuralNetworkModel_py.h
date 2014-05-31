#ifndef NeuralNetworkModel_py_h
#define NeuralNetworkModel_py_h

#include "InputModels/NeuralNetworkModel.h"

#include <boost/python/list.hpp>
using namespace boost::python;

list NeuralNetworkInputs(NeuralNetworkModel& model, InputVector& v1, InputVector& v2) {
    list l;
    float inputs[11];
    model.CreateInputs(v1, v2, inputs);
    for(unsigned int i = 0; i < 11; i++) {
        l.append(inputs[i]);
    }

    return l;
}

#endif
