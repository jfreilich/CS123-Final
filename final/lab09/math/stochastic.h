#ifndef STOCHASTIC_H
#define STOCHASTIC_H
#include "CS123Algebra.h"
//#include <random>
#include <cstdlib>


typedef struct randomVals{
    Vector4 v;
    Vector4 p;
    double t;
    double s;
    Vector4 r;
    Vector4 g;
    Vector4 b;
}random_vals_t;

class stochastic
{

public:
    stochastic();
    static Vector4 position(random_vals_t * rv);
};

#endif // STOCHASTIC_H
