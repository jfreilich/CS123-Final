#ifndef PLANET_H
#define PLANET_H
#include "sphere.h"
#include <stdio.h>
#include <iostream>
using namespace std;

class Planet
{
public:
    Planet(triangle_t* low,int lowsize, triangle_t* mid,int midsize, triangle_t* high, int highsize);
    virtual ~Planet();
    void displace_surface();
    triangle_t* get_tessellation(int tessellation);
private:
    triangle_t* m_low, m_mid, m_high;
};

#endif // PLANET_H
