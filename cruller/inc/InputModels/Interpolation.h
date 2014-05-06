#include "InputModels/InputVector.h"

InputVector SpatialInterpolation(InputVector& iv, unsigned int Nsteps);

InputVector CubicSplineInterpolation(InputVector& iv, unsigned int Nsteps);

InputVector BezierInterpolation(InputVector& iv, unsigned int Nsteps);

InputVector BezierSloppyInterpolation(InputVector& iv, unsigned int Nsteps);
