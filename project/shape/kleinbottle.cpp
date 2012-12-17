#include "kleinbottle.h"
KleinBottle::KleinBottle() :Shape() {
}


void KleinBottle::tesselate(int p1, int p2){
    if (m_triangles){
        delete[] m_triangles;
    }
    if (p1<2){
        p1=10;
    }
    if(p2<3){
        p2=10;
    }
    m_size=2*p2*p1;
    m_triangles=new triangle[m_size];
    double x=0;
    double y=0;
    double z=0;
    int index=0;
    for (int a=0;a<p2;a++){
        for (int b =0;b<p1;b++){
            triangle tri;
            float u=a*M_PI*2.0/p2;
            float v=b*M_PI*2.0/p1;
            for (int m=0;m<=2;m++){
                if (m==1){
                    u=u+M_PI*2.0/p2;
                }
                if (m==2){
                    v=v+M_PI*2.0/p1;
                }
                y=-0.5+cos(u)*((cos(u/2.0)*(pow(2.0,0.5)+cos(v)))+sin(u/2.0)*sin(v)*cos(v));
                z=-0.5+sin(u)*(cos(u/2.0)*(pow(2.0,0.5)+cos(v))+sin(u/2.0)*sin(v)*cos(v));
                x=-0.5+-sin(u/2.0)*(pow(2.0,0.5)+cos(v))+cos(u/2.0)*sin(v)*cos(v);
                tri.coordinates[3*m]=x;
                tri.coordinates[3*m+1]=y;
                tri.coordinates[3*m+2]=z;
            }
            this->normal_vector(tri.coordinates,tri.normals);
            m_triangles[index]=tri;
            index++;
            triangle tri2;
            for (int m=0;m<=2;m++){
                if (m==1){
                    u=u-M_PI*2.0/p2;
                }
                if (m==2){
                    v=v-M_PI*2.0/p1;
                }
                y=-0.5+cos(u)*((cos(u/2.0)*(pow(2.0,0.5)+cos(v)))+sin(u/2.0)*sin(v)*cos(v));
                z=-0.5+sin(u)*(cos(u/2.0)*(pow(2.0,0.5)+cos(v))+sin(u/2.0)*sin(v)*cos(v));
                x=-0.5+-sin(u/2.0)*(pow(2.0,0.5)+cos(v))+cos(u/2.0)*sin(v)*cos(v);
                tri2.coordinates[3*m]=x;
                tri2.coordinates[3*m+1]=y;
                tri2.coordinates[3*m+2]=z;
            }
            this->normal_vector(tri2.coordinates,tri2.normals);
            m_triangles[index]=tri2;
            index++;
        }
    }
}
void KleinBottle::normal_vector(double points[9], double normal[9]){
    double x1=points[0];
    double x2=points[3];
    double x3=points[6];
    double y1=points[1];
    double y2=points[4];
    double y3=points[7];
    double z1=points[2];
    double z2=points[5];
    double z3=points[8];
    double b1=x2-x1;
    double c1=x3-x1;
    double b2=y2-y1;
    double c2=y3-y1;
    double b3=z2-z1;
    double c3=z3-z1;
    double x=b2*c3-c2*b3;
    double y=b3*c1-b1*c3;
    double z=b1*c2-c1*b2;
    double size= pow(x*x+y*y+z*z,0.5);
    x=x/size;
    y=y/size;
    z=z/size;
    for(int i =0;i<3;i++){
        normal[3*i]=x;
        normal[3*i+1]=y;
        normal[3*i+2]=z;
    }
}

