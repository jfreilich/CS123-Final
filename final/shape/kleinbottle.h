#ifndef KLEINBOTTLE_H
#define KLEINBOTTLE_H
#include "Shape.h"

class KleinBottle :public Shape
{
public:
    KleinBottle();
    void tesselate(int p1,int p2);
protected:
void normal_vector(double points[9], double normal[9]);
};

#endif // KLEINBOTTLE_H
