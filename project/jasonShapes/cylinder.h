#ifndef CYLINDER_H
#define CYLINDER_H

#include "triangle.h"

class Cylinder
{
public:
    Cylinder();
    virtual ~Cylinder();
    Triangle** generate(int tess, int slices, int &num);

protected:
    int m_currentTess;
    int m_currentSlices;
    Triangle** m_triangles;
    Point* getPoint(float angle, float distance, float y, float scale);
};

#endif // CYLINDER_H
