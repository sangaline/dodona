#include "Polygon.h"

using namespace std;

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

double Polygon::VertexX(unsigned int i) {
    return vertices[i].first;
}

double Polygon::VertexY(unsigned int i) {
    return vertices[i].second;
}

unsigned int Polygon::VertexCount() {
    return vertices.size();
}

double Polygon::LeftExtreme() {
    double extreme = vertices[0].first;
    for(unsigned int i = 1; i < vertices.size(); i++) {
        if(vertices[i].first < extreme) {
            extreme = vertices[i].first;
        }
    }
    return extreme;
}

double Polygon::RightExtreme() {
    double extreme = vertices[0].first;
    for(unsigned int i = 1; i < vertices.size(); i++) {
        if(vertices[i].first > extreme) {
            extreme = vertices[i].first;
        }
    }
    return extreme;
}

double Polygon::TopExtreme() {
    double extreme = vertices[0].second;
    for(unsigned int i = 1; i < vertices.size(); i++) {
        if(vertices[i].second > extreme) {
            extreme = vertices[i].second;
        }
    }
    return extreme;
}

double Polygon::BottomExtreme() {
    double extreme = vertices[0].second;
    for(unsigned int i = 1; i < vertices.size(); i++) {
        if(vertices[i].second < extreme) {
            extreme = vertices[i].second;
        }
    }
    return extreme;
}
