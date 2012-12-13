#include "stochastic.h"

stochastic::stochastic()
{
}

Vector4 stochastic::position(random_vals_t * rv, Vector3 cam_pos){
    int x=rand();
    x=x%600;
    x=x-300;
    //x+=cam_pos.x;
    int y=rand();
    y=y%600;
    y=y-300;
    //y+=cam_pos.y;
    int z=250;
    //z+=cam_pos.z;
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
