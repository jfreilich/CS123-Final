#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"

class Triangle
{
    public:
        Triangle(Point **normal, Point **pointsa);
        Triangle(Point *normal, Point **pointsa);

        Point** points; //fake 2d array
        Point** vector;

    virtual ~Triangle();
};

#endif // TRIANGLE_H
