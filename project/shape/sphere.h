#ifndef SPHERE_H
#define SPHERE_H
#include "Shape.h"
class Sphere : public Shape
{
public:
    Sphere();
void tesselate(int p1, int p2);
    protected:
    void normal_vector(double points[9], double normal[9]);

};

#endif // SPHERE_H
