#include "Polygon.h"

using namespace std;

Polygon::Polygon() {

}

Polygon::Polygon(const Polygon& p) {
    for(unsigned int i = 0; i < p.VertexCount(); i++) {
        AddVertex( p.VertexX(i), p.VertexY(i) );
    }
}

void Polygon::Reset() {
    vertices.clear();
}

void Polygon::Translate(double x, double y) {
    for(unsigned int i = 0; i < vertices.size(); i++) {
        vertices[i].first += x;
        vertices[i].second += y;
    }
}

unsigned int Polygon::AddVertex(double x, double y) {
    vertices.push_back( make_pair(x, y) );
    return vertices.size();
}

unsigned int Polygon::ReplaceVertex(unsigned int i, double x, double y) {
    vertices[i] = make_pair(x, y);
    return vertices.size();
}

double Polygon::VertexX(unsigned int i) const {
    return vertices[i].first;
}

double Polygon::VertexY(unsigned int i) const {
    return vertices[i].second;
}

unsigned int Polygon::VertexCount() const {
    return vertices.size();
}

double Polygon::LeftExtreme() const {
    double extreme = vertices[0].first;
    for(unsigned int i = 1; i < vertices.size(); i++) {
        if(vertices[i].first < extreme) {
            extreme = vertices[i].first;
        }
    }
    return extreme;
}

double Polygon::RightExtreme() const {
    double extreme = vertices[0].first;
    for(unsigned int i = 1; i < vertices.size(); i++) {
        if(vertices[i].first > extreme) {
            extreme = vertices[i].first;
        }
    }
    return extreme;
}

double Polygon::TopExtreme() const {
    double extreme = vertices[0].second;
    for(unsigned int i = 1; i < vertices.size(); i++) {
        if(vertices[i].second > extreme) {
            extreme = vertices[i].second;
        }
    }
    return extreme;
}

double Polygon::BottomExtreme() const {
    double extreme = vertices[0].second;
    for(unsigned int i = 1; i < vertices.size(); i++) {
        if(vertices[i].second < extreme) {
            extreme = vertices[i].second;
        }
    }
    return extreme;
}

//based on the crossing number/even-odd rule algorithm
//note: basically undefined behavior if it's on an edge
bool Polygon::IsInside(double x, double y) {
    unsigned int crossings = 0;
    for(unsigned int i = 0; i < vertices.size(); i++) {
        const double x1 = i>0 ? vertices[i-1].first : vertices[vertices.size()-1].first;
        const double y1 = i>0 ? vertices[i-1].second : vertices[vertices.size()-1].second;
        const double x2 = vertices[i].first;
        const double y2 = vertices[i].second;
        if( ( y >= y1 && y <= y2) || (y >= y2 && y <= y1) ) {
            const double intersection_x = y1==y2 ? 0.5*(x1+x2) : ((y-y1)*(x2-x1)/(y2-y1)) + x1;
            if(intersection_x < x) {
                crossings++;
            }
        }
    }
    if(crossings%2 == 1) {
        return true;
    }
    return false;
}
