#include "cone.h"
#include "Shape.h"

Cone::Cone() :Shape(){
}
void Cone::normal_vector(double points[9], double normal[9]){
    double rt5=pow(5.0,0.5);

    for(int i =0;i<3;i++){
        double x=points[3*i];
        double y=points[3*i+1];
        double z=points[3*i+2];
        double r=pow(x*x+z*z,0.5);
        if(y==-0.5&&r==0.5){
            //cross product
            double x1=points[0];
            double x2=points[3];
            double x3=points[6];
            double z1=points[2];
            double z2=points[5];
            double z3=points[8];
            double b1=x2-x1;
            double c1=x3-x1;
            double b3=z2-z1;
            double c3=z3-z1;
            double y4=b3*c1-b1*c3;
            if (y4>0){
                x=2.0*x/(r*rt5);
                z=2.0*z/(r*rt5);
                y=1.0/rt5;
            }
            else{
                y=-1;
                x=0;
                z=0;
            }
        }
        else if(y==-0.5){
            y=-1;
            z=0;
            x=0;
        }
        else{
            if(y==0.5){
                x=(points[0]+points[3])/2.0;
                z=(points[2]+points[5])/2.0;
                r=pow(x*x+z*z,0.5);
            }
             x=2.0*x/(r*rt5);
             z=2.0*z/(r*rt5);
             y=1.0/rt5;

        }
        normal[3*i]=x;
        normal[3*i+1]=y;
        normal[3*i+2]=z;
    }
    fflush(stdout);
}
void Cone::tesselate(int p1, int p2){
    if (m_triangles){
        delete[] m_triangles;
    }
    if(p2<3){
        p2=3;
    }
     m_size=2*p2*(2*p1-1);
    m_triangles=new triangle[m_size];
    double x=0;
    double y=0;
    double z=0;
    int index=0;
    for (int a=0;a<p2;a++){
        for (int b =0;b<p1;b++){
            triangle tri;
            float ang=a*M_PI*2.0/p2;
            for (int m=0;m<=2;m++){
                if (m==1){
                    ang=ang+M_PI*2.0/p2;
                }
                x=0.5*sin(ang)*(p1-b-m/2)/(float) p1;
                y=(-0.5+(b+(m/2))/(float) p1);

                z=0.5*cos(ang)*(p1-b-m/2)/(float) p1;
                tri.coordinates[3*m]=x;
                tri.coordinates[3*m+1]=y;
                tri.coordinates[3*m+2]=z;
            }
            this->normal_vector(tri.coordinates,tri.normals);
            m_triangles[index]=tri;
            index++;
            if(b<p1-1){
                triangle tri2;
                ang=ang-M_PI*2.0/p2;
                for (int m=1;m<=3;m++){
                    if (m>1){
                        x=0.5*sin(ang)*(p1-b-1+m/3)/(float) p1;
                        y=(-0.5+(b+1-(m/3))/(float) p1);
                        z=0.5*cos(ang)*(p1-b-1+m/3)/(float) p1;
                    }
                    tri2.coordinates[3*(m-1)]=x;
                    tri2.coordinates[3*m-2]=y;
                    tri2.coordinates[3*m-1]=z;
                }
                this->normal_vector(tri2.coordinates,tri2.normals);
                m_triangles[index]=tri2;
                index++;
            }
        }
    }

    for (int a=0;a<index;a++){
        triangle tri=m_triangles[a];
        for (int b=1;b<=3;b++){
            tri.coordinates[3*b-2]=-0.5;
        }
        float tempx=tri.coordinates[3];
        tri.coordinates[3]=tri.coordinates[6];
        tri.coordinates[6]=tempx;
        float tempz=tri.coordinates[5];
        tri.coordinates[5]=tri.coordinates[8];
        tri.coordinates[8]=tempz;
        this->normal_vector(tri.coordinates,tri.normals);
        m_triangles[a+index]=tri;
    }
}

