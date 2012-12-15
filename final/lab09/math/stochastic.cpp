#include "stochastic.h"

stochastic::stochastic()
{
}

Vector4 stochastic::position(random_vals_t * rv, Vector3 cam_pos){
    int x=rand();
    x=x%20000;
    x=x-10000;
    int y=rand();
    y=y%20000;
    y=y-10000;
//    y=rand()%1500;
  // x=rand()%1500;
    int z=rand()%1000;
    z+=10000;
    Vector4 p=Vector4(x, y, z,0);
    //rv->p=rv->p/25.0;
    return p;

}
Vector4 stochastic::velocity(random_vals_t * rv){
    int x=rand();
    x=x%2000;
    x=x-1000;
    int y=rand();
    y=y%2000;
    y=y-1000;
    int z=rand();
    z=z%5000;
    z=z-5000;
    Vector4 p=Vector4(x, y, z,0);
    //rv->p=rv->p/25.0;
    return p;

}
