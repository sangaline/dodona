#ifndef SimpleInterpolationModel_py_h
#define SimpleInterpolationModel_py_h

#include "InputModels/SimpleInterpolationModel.h"

#include <boost/python/wrapper.hpp>
#include <boost/python/pure_virtual.hpp>
#include <boost/python/call_method.hpp>

using namespace boost::python;

/******* SimpleInterpolationModel Callback **************/
struct SimpleInterpolationModelCallback : SimpleInterpolationModel {

    SimpleInterpolationModelCallback(PyObject *p)
        : SimpleInterpolationModel(), self(p) {}
    SimpleInterpolationModelCallback(PyObject *p, const SimpleInterpolationModel& m) 
        : SimpleInterpolationModel(), self(p) {}

    InputVector Interpolation(InputVector& iv, unsigned int N) const {
        return call_method<InputVector>(self, "Interpolation", iv, N);
    }
    static InputVector default_Interpolation(const SimpleInterpolationModel& self_, InputVector& iv, unsigned int N)  {
        return self_.SimpleInterpolationModel::Interpolation(iv, N);
    }

  private:
    PyObject* self; // 1
};
/********************************************************/

#endif
