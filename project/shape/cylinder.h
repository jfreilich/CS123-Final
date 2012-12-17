#ifndef CYLINDER_H
#define CYLINDER_H
#include "Shape.h"

class Cylinder : public Shape
{
public:
    Cylinder();
    void tesselate(int p1, int p2);

protected:
void normal_vector(double points[9], double normal[9]);
};

#endif // CYLINDER_H
