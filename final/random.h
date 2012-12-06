#ifndef RANDOM_H
#define RANDOM_H
#include "math/CS123Algebra.h"
#include<math.h>
#include<stdlib.h>
typedef struct rand_param{
   Vector4 v;
   Vector4 p;
}rand_param_t;

class random
{
public:
    random();

    double radius(rand_param_t param);

};

#endif // RANDOM_H
