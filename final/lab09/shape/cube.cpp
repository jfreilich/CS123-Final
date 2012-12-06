#include "cube.h"
#include "Shape.h"
#include <stdio.h>
#include <math.h>


Cube::Cube() :Shape() {
}
void Cube::normal_vector(double points[9], double normal[9]){
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
void Cube::tesselate(int p1, int p2){
    if (m_triangles){
        delete[] m_triangles;
    }
    m_size=12*p1*p1;
    m_triangles=new triangle[m_size];
    double x=0;
    double y=0;
    double z=0;
    int index=0;
    for (int k=0;k<=1;k++){
        for (int a=0;a<p1;a++){
            for (int b =0;b<p1;b++){
                triangle tri;
                for (int m=1;m<=3;m++){
                    x=pow(-1,k)*(-0.5+(b+m/3)/(float) p1);
                    y=0.5-(a+m/2)/(float) p1;
                    z=pow(-1,k)*0.5;
                    tri.coordinates[3*(m-1)]=x;
                    tri.coordinates[3*m-2]=y;
                    tri.coordinates[3*m-1]=z;
                }
              this->normal_vector(tri.coordinates,tri.normals);
              m_triangles[index]=tri;
              index++;
            }
        }
        for (int a=0;a<p1;a++){
            for (int b =0;b<p1;b++){
                triangle tri;
                for (int m=1;m<=3;m++){
                    x=pow(-1,k)*(-0.5+(b+1-m/3)/(float) p1);
                    y=0.5-(1+a-m/2)/(float) p1;
                    z=pow(-1,k)*0.5;
                    tri.coordinates[3*(m-1)]=x;
                    tri.coordinates[3*m-2]=y;
                    tri.coordinates[3*m-1]=z;
                }
                            this->normal_vector(tri.coordinates,tri.normals);
              m_triangles[index]=tri;
              index++;
            }
        }
    }
    for (int k=0;k<=1;k++){
        for (int a=0;a<p1;a++){
            for (int b =0;b<p1;b++){
                triangle tri;
                for (int m=1;m<=3;m++){
                    x=pow(-1,k)*-0.5;
                    y=0.5-(a+m/2)/(float) p1;
                    z=pow(-1,k)*(-0.5+(b+m/3)/(float) p1);
                    tri.coordinates[3*(m-1)]=x;
                    tri.coordinates[3*m-2]=y;
                    tri.coordinates[3*m-1]=z;
                }
                            this->normal_vector(tri.coordinates,tri.normals);
              m_triangles[index]=tri;
              index++;
            }
        }
        for (int a=0;a<p1;a++){
            for (int b =0;b<p1;b++){
                triangle tri;
                for (int m=1;m<=3;m++){
                    x=pow(-1,k)*-0.5;
                    y=0.5-(1+a-m/2)/(float) p1;
                    z=pow(-1,k)*(-0.5+(b+1-m/3)/(float) p1);
                    tri.coordinates[3*(m-1)]=x;
                    tri.coordinates[3*m-2]=y;
                    tri.coordinates[3*m-1]=z;
                }
                            this->normal_vector(tri.coordinates,tri.normals);
              m_triangles[index]=tri;
              index++;
            }
        }
    }
    for (int k=0;k<=1;k++){
        for (int a=0;a<p1;a++){
            for (int b =0;b<p1;b++){
                triangle tri;
                for (int m=1;m<=3;m++){
                    x=pow(-1,k)*(-0.5+(b+m/3)/(float) p1);
                    y=pow(-1,k)*(-0.5);
                    z=0.5-(a+m/2)/(float) p1;
                    tri.coordinates[3*(m-1)]=x;
                    tri.coordinates[3*m-2]=y;
                    tri.coordinates[3*m-1]=z;
                }
                            this->normal_vector(tri.coordinates,tri.normals);
              m_triangles[index]=tri;
              index++;
            }
        }
        for (int a=0;a<p1;a++){
            for (int b =0;b<p1;b++){
                triangle tri;
                for (int m=1;m<=3;m++){
                    x=pow(-1,k)*(-0.5+(b+1-m/3)/(float) p1);
                    y=pow(-1,k)*(-0.5);
                    z=0.5-(1+a-m/2)/(float) p1;
                    tri.coordinates[3*(m-1)]=x;
                    tri.coordinates[3*m-2]=y;
                    tri.coordinates[3*m-1]=z;
                }
              this->normal_vector(tri.coordinates,tri.normals);
              m_triangles[index]=tri;
              index++;
            }
        }
    }
}


