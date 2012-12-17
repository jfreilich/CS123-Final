#include "triangle.h"
#include "point.h"

/*
 This object deals with the three points and the vectors at
 said points.

 Also an object here, so the deconstructor can be easily dealt with.

 Making sure not to leak.
 */
Triangle::Triangle(Point **normal, Point **pointsa)
{
    vector = normal;
    points = pointsa;
}
Triangle::Triangle(Point *normal, Point **pointsa)
{
    vector = new Point*[3];
    vector[0] = normal;
    vector[1] = normal;
    vector[2] = normal;
    points = pointsa;
}
Triangle::~Triangle()
{
    delete[] vector;
    delete[] points;
}
