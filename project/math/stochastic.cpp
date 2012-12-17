#include "stochastic.h"

stochastic::stochastic()
{
}

Vector4 stochastic::position(random_vals_t * rv, Vector3 cam_pos){
    int x=rand();
    x=x%3000;
    x=x-1500;
    int y=rand();
    y=y%3000;
    y=y-1500;
//    y=rand()%1500;
  // x=rand()%1500;
    int z=rand()%128;
    z+=3072;
    Vector4 p=Vector4(x, y, z,0);
    //rv->p=rv->p/25.0;
    return p;

}
Vector4 stochastic::velocity(random_vals_t * rv){
    int x=rand();
    x=x%64;
    x-=32;
    int y=rand();
    y=y%64;
    y-=32;
    int z=rand();
    z=z%128;
    z-=128;
    Vector4 p=Vector4(x, y, z,0);
    //rv->p=rv->p/25.0;
    return p;

}
