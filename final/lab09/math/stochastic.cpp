#include "stochastic.h"

stochastic::stochastic()
{
}

Vector4 stochastic::position(random_vals_t * rv){
    int x=rand();
    x=x%200;
    x=x-100;
    int y=rand();
    y=y%200;
    y=y-100;
    int z=rand();
    z=z%200;
    z=z-100;
    Vector4 p=Vector4((double) x,(double) y,(double) z,0);
    //rv->p=rv->p/25.0;
    return p/50.0;


}
