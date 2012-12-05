#ifndef RANDOM_H
#define RANDOM_H

#include<math.h>
#include<stdlib.h>
typedef struct rand_param{
    double x;
    double y;
    double z;
    double xV;
    double yV;
    double zV;
    double p1;
    double p2;
    double p3;
}rand_param_t;

class random
{
public:
    random();

    double radius(rand_param_t param);

};

#endif // RANDOM_H
