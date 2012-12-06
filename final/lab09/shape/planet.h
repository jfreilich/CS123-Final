#ifndef PLANET_H
#define PLANET_H

#include "math/vector.h"
#include <iostream>
#include "jasonShapes/triangle.h"

using namespace std;

class Planet
{
public:
    Planet(Triangle** mid, int numMid);
    virtual ~Planet();

    void modifyColor(Vector3 baseR, Vector3 baseG, Vector3 baseB);
    Triangle** getMid(int &num);
private:
    Triangle** m_mid;
    int m_numMid;
};

#endif // PLANET_H
