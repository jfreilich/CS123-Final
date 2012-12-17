#ifndef CUBE_H
#define CUBE_H

#include "triangle.h"

class Cube
{
public:
    Cube();
    virtual ~Cube();
    Triangle** generate(int tess, int &num);

protected:
    int m_currentTess;
    Triangle** m_triangles;
};

#endif // CUBE_H
