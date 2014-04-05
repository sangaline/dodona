#ifndef Polygon_h
#define Polygon_h

#include <vector>
#include <utility>

class Polygon {
    std::vector< std::pair<double, double> > vertices;

  public:
    unsigned int AddVertex(double x, double y);
    unsigned int ReplaceVertex(unsigned int i, double x, double y);
    unsigned int VertexCount();
    double VertexX(unsigned int i);
    double VertexY(unsigned int i);
    double LeftExtreme();
    double RightExtreme();
    double TopExtreme();
    double BottomExtreme();
    void Reset();
};

#endif
