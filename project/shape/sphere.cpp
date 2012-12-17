#include "sphere.h"

Sphere::Sphere():Shape()
{
}
void Sphere::normal_vector(double points[9], double normal[9]){
    for(int i=0;i<3;i++){
        double x=points[3*i];
        double y=points[3*i+1];
        double z=points[3*i+2];
        double r=pow(x*x+y*y+z*z,0.5);
        x=x/r;
        y=y/r;
        z=z/r;
        normal[3*i]=x;
        normal[3*i+1]=y;
        normal[3*i+2]=z;
    }
}

void Sphere::tesselate(int p1, int p2){
    if (m_triangles){
        delete[] m_triangles;
    }
    if (p1<2){
        p1=2;
    }
    if(p2<3){
        p2=3;
    }
    m_size=p2*(2*p1-1);
    m_triangles=new triangle[m_size];
    double x=0;
    double y=0;
    double z=0;
    int index=0;
    for (int a=0;a<p2;a++){
        for (int b =0;b<p1;b++){
            triangle tri;
            float angW=a*M_PI*2.0/p2;
            float angH=(b+1)*M_PI/p1;
            for (int m=0;m<=2;m++){
                if (m==1){
                    angH=angH-M_PI/p1;
                }
                if (m==2){
                    angW=angW+M_PI*2.0/p2;
                }
                x=0.5*sin(angW)*sin(angH);
                y=-0.5*cos(angH);
                z=0.5*cos(angW)*sin(angH);
                tri.coordinates[3*m]=x;
                tri.coordinates[3*m+1]=y;
                tri.coordinates[3*m+2]=z;
            }
            this->normal_vector(tri.coordinates,tri.normals);
            m_triangles[index]=tri;
            index++;
            if(b<p1-1){
                triangle tri2;
                for (int j=0;j<=2;j++){
                    if (j>0){
                        if (j==1){
                            angH=angH+M_PI/p1;
                        }
                        if (j==2){
                            angW=angW-M_PI*2.0/p2;
                        }
                        x=0.5*sin(angW)*sin(angH);
                        y=-0.5*cos(angH);
                        z=0.5*cos(angW)*sin(angH);
                    }
                    tri2.coordinates[3*j]=x;
                    tri2.coordinates[3*j+1]=y;
                    tri2.coordinates[3*j+2]=z;
                }
                this->normal_vector(tri2.coordinates,tri2.normals);
                m_triangles[index]=tri2;
                index++;
            }
        }
    }
}
