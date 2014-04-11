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
