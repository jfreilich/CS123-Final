#ifndef CONE_H
#define CONE_H
#include "Shape.h"
class Cone : public Shape{

    public:

        Cone();
        void tesselate(int p1, int p2);
    protected:
void normal_vector(double points[9], double normal[9]);

};

#endif // Cone_H
