#include "sphere.h"
#include "point.h"
#include "triangle.h"
#include "math.h"

#include <iostream>
using namespace std;

Sphere::Sphere()
{
    m_currentTess = -1;
    m_currentSlices = -1;
    m_triangles = NULL;
    m_PI = 3.1415926535;
}

Sphere::~Sphere() {
    delete[] m_triangles;
}

Triangle** Sphere::generate(int tess, int slices, int &num) {

    tess = max(tess, 2);
    slices = max(slices,3);

    num = (tess-1)*(slices)*2; // total number of triangles, so we know what size array to have

    if (tess == m_currentTess && slices == m_currentSlices)
        // use the previously generated triangles if the settings are the same
        return m_triangles;

    m_currentTess = tess;
    m_currentSlices = slices;


    delete[] m_triangles;

    m_triangles = new Triangle*[num];

    Point** normal;
    Point** points;

    int n = 0;


    // go through lat and lon, and use the getPoint function to generate our points for us!
    // #helperfunctions
    for (int theta=0;theta<slices;theta++) {
        for (int layer=0;layer<tess;layer++) {
            normal = new Point*[3];
            points = new Point*[3];

            if (layer==0) {
                points[2] = getPoint(theta,slices,layer,tess,0.5);
                normal[2] = getPoint(theta,slices,layer,tess,1);

                points[1] = getPoint(theta,slices,layer+1,tess,0.5);
                normal[1] = getPoint(theta,slices,layer+1,tess,1);

                points[0] = getPoint(theta+1,slices,layer+1,tess,0.5);
                normal[0] = getPoint(theta+1,slices,layer+1,tess,1);

                m_triangles[n] = new Triangle(normal,points);
                n++;
            }
            else if (layer+1 == tess) {
                points[0] = getPoint(theta+1,slices,layer,tess,0.5);
                normal[0] = getPoint(theta+1,slices,layer,tess,1);

                points[1] = getPoint(theta,slices,layer+1,tess,0.5);
                normal[1] = getPoint(theta,slices,layer+1,tess,1);

                points[2] = getPoint(theta,slices,layer,tess,0.5);
                normal[2] = getPoint(theta,slices,layer,tess,1);

                m_triangles[n] = new Triangle(normal,points);
                n++;
            }
            else {
                points[0] = getPoint(theta,slices,layer,tess,0.5);
                normal[0] = getPoint(theta,slices,layer,tess,1);

                points[1] = getPoint(theta+1,slices,layer,tess,0.5);
                normal[1] = getPoint(theta+1,slices,layer,tess,1);

                points[2] = getPoint(theta+1,slices,layer+1,tess,0.5);
                normal[2] = getPoint(theta+1,slices,layer+1,tess,1);
                m_triangles[n] = new Triangle(normal,points);
                n++;

                normal = new Point*[3];
                points = new Point*[3];

                points[0] = getPoint(theta,slices,layer,tess,0.5);
                normal[0] = getPoint(theta,slices,layer,tess,1);

                points[1] = getPoint(theta+1,slices,layer+1,tess,0.5);
                normal[1] = getPoint(theta+1,slices,layer+1,tess,1);

                points[2] = getPoint(theta,slices,layer+1,tess,0.5);
                normal[2] = getPoint(theta,slices,layer+1,tess,1);

                m_triangles[n] = new Triangle(normal,points);
                n++;
            }
        }
    }

    return m_triangles;
}

Point* Sphere::getPoint(int o, int maxo, int i, int maxi, float distance) {

    float longitude = 2*m_PI*o*1.0/maxo;
    float latitude = m_PI*i*1.0/maxi;

    Point* p = new Point(distance*sin(latitude)*cos(longitude),distance*cos(latitude),distance*sin(latitude)*sin(longitude),1);

    return p;
}
