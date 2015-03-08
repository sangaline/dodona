#ifndef Polygon_h
#define Polygon_h

#include <vector>
#include <utility>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>

//Forward declaration of class boost::serialization::access
namespace boost {namespace serialization {class access;}}

class Polygon {
    std::vector< std::pair<double, double> > vertices;

  public:
    Polygon();
    Polygon(const Polygon& p);
    bool operator==(const Polygon& other);
    void Translate(double x, double y);
    unsigned int AddVertex(double x, double y);
    unsigned int ReplaceVertex(unsigned int i, double x, double y);
    unsigned int VertexCount() const;
    double VertexX(unsigned int i) const;
    double VertexY(unsigned int i) const;
    double LeftExtreme() const;
    double RightExtreme() const;
    double TopExtreme() const;
    double BottomExtreme() const;
    bool IsInside(double x, double y);
    void Reset();

  private:
    //allow serialization to access non-public data members.
    friend class boost::serialization::access;
    //the implementation *has* to be in the header file
    template<typename Archive> void serialize(Archive& ar, const unsigned int version) {
        ar & vertices; //note that special includes were required to handle vectors and pairs
    }
};

#endif
