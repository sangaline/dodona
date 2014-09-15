#include "InputModels/InputVector.h"

InputVector SpatialInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector CubicSplineInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector BezierInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector BezierSloppyInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector BezierInterpolationV2(InputVector& iv, unsigned int Nsteps);

//helper functions
InputVector QuadraticBezierInterpolation(InputVector& iv, unsigned int Nsteps);
InputVector CombineInputVectors(std::vector<InputVector> IVvect);
double dist2next(InputVector& iv, unsigned int i);
