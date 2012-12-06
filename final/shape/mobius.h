#ifndef MOBIUS_H
#define MOBIUS_H
#include "Shape.h"

class Mobius :public Shape
{
public:
    Mobius();
    void tesselate(int p1,int p2);
protected:
void normal_vector(double points[9], double normal[9]);
void average_normals(int p1, int p2);
};
#endif
