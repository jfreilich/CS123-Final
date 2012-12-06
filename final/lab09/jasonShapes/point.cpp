#include "point.h"

#include <iostream>
using namespace std;


/*
 Decided to make an object Point instead of a struct, just so that I could
 include a couple methods and be more homogenous in my treatment of all of my objects.

 n1 is not used ever, but I'm including it because apparently
 we're going to have another parameter at some point?
 */
Point::Point(float x1, float y1, float z1, float n1)
{
    x = x1;
    y = y1;
    z = z1;
    n = n1;
}

void Point::switchXY() {
    double t;
    t = x;
    x = y;
    y = t;
}
void Point::switchXZ() {
    double t;
    t = x;
    x = z;
    z = t;
}
void Point::switchYZ() {
    double t;
    t = y;
    y = z;
    z = t;
}
