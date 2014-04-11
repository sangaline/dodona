#ifndef Polygon_h
#define Polygon_h

#include <vector>
#include <utility>

class Polygon {
    std::vector< std::pair<double, double> > vertices;

  public:
    Polygon();
    Polygon(const Polygon& p);
    unsigned int AddVertex(double x, double y);
    unsigned int ReplaceVertex(unsigned int i, double x, double y);
    unsigned int VertexCount() const;
    double VertexX(unsigned int i) const;
    double VertexY(unsigned int i) const;
    double LeftExtreme() const;
    double RightExtreme() const;
    double TopExtreme() const;
    double BottomExtreme() const;
    void Reset();
};

#endif
