#include "impliciteq.h"
#include <cstdlib>
#include <cmath>
#include <CS123Algebra.h>


implicitEQ::implicitEQ()
{
    epsilon=0.0;
}
double implicitEQ::quadratic(double a2, double b, double c){//a2 = 2*a, b=-b;
    double temp=b*b-2.0*a2*c;
    double t=0;
    if (temp>0){
        temp=pow(temp,0.5);
    }
    else if(temp==0){//tangent
        return t;
    }
    else{
        return t;
    }
    double t1=(b+temp)/a2;
    double t2=(b-temp)/a2;
    if (t2>EPSILON){
        return t2;
    }
    if (t1>EPSILON){
        return t1;
    }
    return 0;
}

double implicitEQ::sphere(Vector4 p, Vector4 d){
    double a2=d.x*d.x+d.y*d.y+d.z*d.z;
    a2=2.0*a2;
    double b=2.0*(p.x*d.x+p.y*d.y+p.z*d.z);
    b=-b;
    double c=p.x*p.x+p.y*p.y+p.z*p.z-0.25;
    double t = quadratic(a2,b,c);
    return t;
}

double implicitEQ::cube(Vector4 p, Vector4 d){
    int length=0;
    double t=0;
    double t1=(0.5-p.y)/d.y;
    if (t1>EPSILON){
        if (abs(p.x+d.x*t1)<=0.5&&abs(p.z+d.z*t1)<=0.5){
            length++;
            t=t1;
        }
    }
    double t2=(-0.5-p.y)/d.y;
    if(t2>EPSILON){
        if (abs(p.x+d.x*t2)<=0.5&&abs(p.z+d.z*t2)<=0.5){
            if (length){
                t=min(t,t2);
            }
            else{
                t=t2;
            }
            length++;
        }
    }
    if (length==2){
        return t;
    }
    double t3=(0.5-p.x)/d.x;
    if (t3>EPSILON){
        if (abs(p.y+d.y*t3)<=0.5&&abs(p.z+d.z*t3)<=0.5){
            if (length){
                t=min(t,t3);
            }
            else{
                t=t3;
            }
            length++;
        }
    }
    if (length==2){
        return t;
    }
    double t4=(-0.5-p.x)/d.x;
    if (t4>EPSILON){
        if (abs(p.y+d.y*t4)<=0.5&&abs(p.z+d.z*t4)<=0.5){
            if (length){
                t=min(t,t4);
            }
            else{
                t=t4;
            }
            length++;
        }
    }
    if (length==2){
        return t;
    }
    double t5=(0.5-p.z)/d.z;
    if (t5>EPSILON){
        if (abs(p.y+d.y*t5)<=0.5&&abs(p.x+d.x*t5)<=0.5){
            if (length){
                t=min(t,t5);
            }
            else{
                t=t5;
            }
            length++;
        }
    }
    if (length==2){
        return t;
    }
    double t6=(-0.5-p.z)/d.z;
    if (t6>EPSILON){
        if (abs(p.y+d.y*t6)<=0.5&&abs(p.x+d.x*t6)<=0.5){
            if (length){
                t=min(t,t6);
            }
            else{
                t=t6;
            }
            length++;
        }
    }
    return t;
}

double implicitEQ::cylinder(Vector4 p, Vector4 d){
    double a2=d.x*d.x+d.z*d.z;
    a2=2.0*a2;
    double b=2.0*(p.x*d.x+p.z*d.z);
    b=-b;
    double c=p.x*p.x+p.z*p.z-0.25;
    double temp=b*b-2.0*a2*c;
    double t=0;
    double y;
    if (temp>0){
        temp=pow(temp,0.5);
        double t1=(b+temp)/a2;
        double t2=(b-temp)/a2;
        if (t2>EPSILON){
            y=t2*d.y+p.y;
            if (y<0.5-EPSILON&&y>-0.5+EPSILON){
                t=t2;
            }
        }
        if (t1>EPSILON){
            y=t1*d.y+p.y;
            if (y<0.5-EPSILON&&y>-0.5+EPSILON){
                if(t==0){
                    t=t1;
                }
            }
        }
    }
    double t3 = cylinderBase(p,d);
    if (t3>EPSILON){
        if (t==0){
            t=t3;
        }
        else{
            t=std::min(t,t3);
        }
    }
    return t;
}

double implicitEQ::cylinderBase(Vector4 p, Vector4 d){
    double t1=(0.5-p.y)/d.y;
    double t2=(-0.5-p.y)/d.y;
    double t=0;
    double x=0;
    double z=0;
    if (t2>EPSILON){
        x=d.x*t2+p.x;
        z=d.z*t2+p.z;
        if (x*x+z*z<=0.25){
            t=t2;
        }
    }
    if (t1>EPSILON){
        x=d.x*t1+p.x;
        z=d.z*t1+p.z;
        if (x*x+z*z<=0.25){
            if(t==0){
                t=t1;
            }
            else{
                t=std::min(t,t1);
            }
        }
    }
    return t;
}

double implicitEQ::cone(Vector4 p, Vector4 d){
    double a2=d.x*d.x-d.y*d.y/4.0+d.z*d.z;
    a2=2.0*a2;
    double b=2.0*p.x*d.x-p.y*d.y/2.0+d.y/4.0+2.0*p.z*d.z;
    b=-b;
    double c=p.x*p.x-p.y*p.y/4.0+p.y/4.0-1.0/16.0+p.z*p.z;
    double temp=b*b-2.0*a2*c;
    double t=0;
    double x;
    double y;
    double z;
    if (temp>0){
        temp=pow(temp,0.5);
        double t1=(b+temp)/a2;
        double t2=(b-temp)/a2;
        if (t2>EPSILON){
            y=t2*d.y+p.y;
            if (y<=0.5&&y>=-0.5){
                t=t2;
            }
        }
        if (t1>EPSILON){
            y=t1*d.y+p.y;
            if (y<=0.5&&y>=-0.5){
                if(t==0){
                    t=t1;
                }
            }
        }
    }
    double t3 = coneBase(p,d);
    if (t3>EPSILON){
        x=d.x*t3+p.x;
        z=d.z*t3+p.z;
        if (x*x+z*z<=0.25){
            if (t==0){
                t=t3;
            }
            else{
                t=std::min(t,t3);
            }
        }
    }
    return t;
}

double implicitEQ::coneBase(Vector4 p, Vector4 d){
    double t=(-0.5-p.y)/d.y;
    return t;
}

Vector4 implicitEQ::coneNormal(double x, double y, double z){
    double y2=y;
    y2=fabs(y2);
    if (y2<0.4999){
        double rt5=pow(5.0,0.5);
        double r=pow(x*x+z*z,0.5);
        x=2.0*x/(r*rt5);
        z=2.0*z/(r*rt5);
        y=1.0/rt5;
        return Vector4(x,y,z,0);

    }
    else{
        return Vector4(0,2.001*y,0,0);
    }
}
Vector4 implicitEQ::cubeNormal(double x, double y, double z){
    int x2=2.00001*x;
    int y2=2.00001*y;
    int z2=2.00001*z;
    Vector4 v=Vector4(x2,y2,z2,0);
    v.normalize();
    return v;
}
Vector4 implicitEQ::cylinderNormal(double x, double y, double z){
    if (y<0.5-EPSILON&&y>-0.5+EPSILON){
        return Vector4(2.0*x,0,2.0*z,0);
    }
    else{
        return Vector4(0,2.0*y,0,0);
    }
}
Vector4 implicitEQ::sphereNormal(double x, double y, double z){
    return Vector4(2.0*x,2.0*y,2.0*z,0);
}
