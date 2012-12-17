#ifndef N_H
#define N_H
#include "Shape.h"

class N : public Shape
{
public:
    N();
void tesselate(int p1, int p2);
protected:
void normal_vector(double points[9], double normal[9]);

};

#endif // N_H
