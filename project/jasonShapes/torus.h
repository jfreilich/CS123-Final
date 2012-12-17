#ifndef TORUS_H
#define TORUS_H

#include "triangle.h"

class Torus
{
public:
    Torus();
    virtual ~Torus();
    Triangle** generate(int tess, int slices, int three, int hollow, int &num);

protected:
    int m_currentTess;
    int m_currentSlices;
    int m_hollow;
    float m_PI;
    float m_three;
    Triangle** m_triangles;
    Point* getPoint(int o, int maxo, int i, int maxi, float higrader);
};

#endif // TORUS_H
