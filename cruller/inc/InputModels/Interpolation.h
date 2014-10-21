#include "InputVector.h"
//#include "InputModels/InputVector.h"

InputVector SpatialInterpolation(InputVector& iv, unsigned int Nsteps);

InputVector HermiteCubicSplineInterpolationBase(InputVector& iv, unsigned int Nsteps, bool monotonic);
InputVector HermiteCubicSplineInterpolation(InputVector& iv, unsigned int Nsteps); 
InputVector MonotonicCubicSplineInterpolation(InputVector& iv, unsigned int Nsteps);

InputVector CubicSplineInterpolationBase(InputVector& iv, unsigned int Nsteps, bool mod);
InputVector CubicSplineInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector ModCubicSplineInterpolation(InputVector& iv, unsigned int Nsteps);

InputVector BezierInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector BezierSloppyInterpolation(InputVector& iv, unsigned int Nsteps);
