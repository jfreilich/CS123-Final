#ifndef SPHERE_H
#define SPHERE_H

#include "triangle.h"

class Sphere
{
public:
    Sphere();
    virtual ~Sphere();
    Triangle** generate(int tess, int slices, int &num);

protected:
    int m_currentTess;
    int m_currentSlices;
    float m_PI;
    Triangle** m_triangles;
    Point* getPoint(int o, int maxo, int i, int maxi, float distance);
};

#endif // SPHERE_H
