#include "mobius.h"

Mobius::Mobius():Shape()
{

}
void Mobius::normal_vector(double points[9], double normal[9]){
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

void Mobius::average_normals(int p1, int p2){
    triangle tri, tri2, tri3;
    for (int i=0;i<m_size;i++){
        tri=m_triangles[i];
        tri2=m_triangles[(i+2*p1)%m_size];
        tri3=m_triangles[(i-2*p1+m_size)%m_size];
        for (int a=6;a<9;a++){
            tri.normals[a]=(tri.normals[a]+tri3.normals[a])/2.0;
        }
        for(int a=0;a<3;a++){
            tri.normals[a]=(tri.normals[a]+tri3.normals[a])/2.0;
        }
        for(int a=3;a<6;a++){
            tri.normals[a]=(tri.normals[a]+tri2.normals[a])/2.0;
        }
m_triangles[i]=tri;
    }

}

void Mobius::tesselate(int p1, int p2){
    if (m_triangles){
        delete[] m_triangles;
    }
    if(p2<3){
        p2=3;
    }
    m_size=p1*4*p2;
    m_triangles=new triangle[m_size];
    double x=0;
    double y=0;
    double z=0;
    int index=0;
    for (int a=0;a<p2;a++){
        for (int b =0;b<p1;b++){
            triangle tri;
            float ang=a*M_PI*2.0/p2;
            float s=-0.25+b/((2.0)*(float) p1);
            for (int m=0;m<=2;m++){
                if (m==1){
                    ang=ang+M_PI*2.0/p2;
                }
                if (m==2){
                    s=s+0.5/p1;
                }
                x=s*sin(ang/2.0);
                y=(0.5+s*cos(ang/2.0))*cos(ang);
                z=(0.5+s*cos(ang/2.0))*sin(ang);
                tri.coordinates[3*m]=x;
                tri.coordinates[3*m+1]=y;
                tri.coordinates[3*m+2]=z;
            }
            this->normal_vector(tri.coordinates,tri.normals);
            m_triangles[index]=tri;
            index++;
            triangle tri3=tri;
            tri3.coordinates[3]=tri.coordinates[6];
            tri3.coordinates[4]=tri.coordinates[7];
            tri3.coordinates[5]=tri.coordinates[8];
            tri3.coordinates[6]=tri.coordinates[3];
            tri3.coordinates[7]=tri.coordinates[4];
            tri3.coordinates[8]=tri.coordinates[5];
            this->normal_vector(tri3.coordinates,tri3.normals);
            m_triangles[index]=tri3;
            index++;
            triangle tri2;
            for (int m=1;m<=3;m++){
                if (m==2){
                    ang=ang-M_PI*2.0/p2;
                }
                if (m==3){
                    s=s-0.5/p1;
                }
                x=s*sin(ang/2.0);
                y=(0.5+s*cos(ang/2.0))*cos(ang);
                z=(0.5+s*cos(ang/2.0))*sin(ang);
                tri2.coordinates[3*(m-1)]=x;
                tri2.coordinates[3*m-2]=y;
                tri2.coordinates[3*m-1]=z;
            }
            this->normal_vector(tri2.coordinates,tri2.normals);
            m_triangles[index]=tri2;
            index++;
            triangle tri4=tri2;
            tri4.coordinates[3]=tri2.coordinates[6];
            tri4.coordinates[4]=tri2.coordinates[7];
            tri4.coordinates[5]=tri2.coordinates[8];
            tri4.coordinates[6]=tri2.coordinates[3];
            tri4.coordinates[7]=tri2.coordinates[4];
            tri4.coordinates[8]=tri2.coordinates[5];
            this->normal_vector(tri4.coordinates,tri4.normals);
            m_triangles[index]=tri4;
            index++;
        }
    }
    //this->average_normals(p1, p2);
}
