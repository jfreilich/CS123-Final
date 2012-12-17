#ifndef IMPLICITEQ_H
#define IMPLICITEQ_H
#include "CS123Algebra.h"
class implicitEQ
{
public:
    implicitEQ();
    double sphere(Vector4 p, Vector4 d);
    double cube(Vector4 p, Vector4 d);
    double cylinder(Vector4 p, Vector4 d);
    double cylinderBase(Vector4 p, Vector4 d);
    double cone(Vector4 p, Vector4 d);
    double coneBase(Vector4 p, Vector4 d);
    double quadratic(double a2, double b, double c);//a2 = 2*a, b=-b;
    Vector4 sphereNormal(double x, double y, double z);
    Vector4 cylinderNormal(double x, double y, double z);
    Vector4 coneNormal(double x, double y, double z);
    Vector4 cubeNormal(double x, double y, double z);
    double epsilon;
};

#endif // IMPLICITEQ_H
