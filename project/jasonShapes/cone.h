#ifndef CONE_H
#define CONE_H

#include "triangle.h"

class Cone
{
public:
    Cone();
    virtual ~Cone();
    Triangle** generate(int tess, int slices, int &num);

protected:
    int m_currentTess;
    int m_currentSlices;
    Triangle** m_triangles;
    Point* getPoint(float angle, float distance, float y, float scale);
};

#endif // CONE_H
