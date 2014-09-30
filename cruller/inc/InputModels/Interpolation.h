#include "InputModels/InputVector.h"

InputVector SpatialInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector HermiteCubicInterpolation(InputVector& iv, unsigned int Nsteps, bool monotonic);
InputVector MonotonicCubicSplineInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector CubicSplineInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector CubicSplineInterpolationV2(InputVector& iv, unsigned int Nsteps);
InputVector BezierInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector BezierSloppyInterpolation(InputVector& iv, unsigned int Nsteps);
