#ifndef CUBE_H
#define CUBE_H
#include "Shape.h"
class Cube : public Shape{

    public:

        Cube();

        void tesselate(int p1, int p2);

    protected:
void normal_vector(double points[9], double normal[9]);

};

#endif // CUBE_H
