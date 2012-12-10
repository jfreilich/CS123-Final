#include "stochastic.h"

stochastic::stochastic()
{
}

Vector4 stochastic::position(random_vals_t * rv){
    int x=rand();
    x=x%600;
    x=x-300;
    int y=rand();
    y=y%600;
    y=y-300;
    int z=250;
    Vector4 p=Vector4(x, y, z,0);
    //rv->p=rv->p/25.0;
    return p;

}
Vector4 stochastic::velocity(random_vals_t * rv){
    int x=rand();
    x=x%200;
    x=x-100;
    int y=rand();
    y=y%200;
    y=y-100;
    int z=rand();
    z=z%500;
    z=z-500;
    Vector4 p=Vector4(x, y, z,0);
    //rv->p=rv->p/25.0;
    return p;

}
