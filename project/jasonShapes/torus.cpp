#include "torus.h"
#include "point.h"
#include "triangle.h"
#include <cmath>
#include <iostream>
using namespace std;

Torus::Torus()
{
    m_currentTess = -1;
    m_currentSlices = -1;
    m_triangles = NULL;
    m_PI = 3.1415926535;
    m_three = 1;
    m_hollow = 0;
}

Torus::~Torus() {
    delete[] m_triangles;
}

Triangle** Torus::generate(int tess, int slices, int three, int hollow, int &num) {
    /*
     This guy uses the third parameter to determine the inner radius of the torus.
     */


    three = min(max(1, three), 65);
    tess = max(tess, 3);
    slices = max(slices,3);
    hollow = min(max(hollow, 0), 1);

    num = (tess)*(slices)*2;

    if (tess == m_currentTess && slices == m_currentSlices && three == m_three && m_hollow == hollow)
        return m_triangles;

    m_currentTess = tess;
    m_currentSlices = slices;
    m_three = three;
    m_hollow = hollow;

    delete[] m_triangles;

    m_triangles = new Triangle*[num];

    Point** normal;
    Point** points;

    int n = 0;

    for (int theta=0;theta<slices;theta++) {
        for (int miniT=0;miniT<tess;miniT++) {
            normal = new Point*[3];
            points = new Point*[3];

            points[hollow*2] = getPoint(theta,slices,miniT,tess,1);
            normal[hollow*2] = getPoint(theta,slices,miniT,tess,0);

            points[1] = getPoint(theta+1,slices,miniT,tess,1);
            normal[1] = getPoint(theta+1,slices,miniT,tess,0);

            points[(int)abs(hollow*2-2.0)] = getPoint(theta+1,slices,miniT+1,tess,1);
            normal[(int)abs(hollow*2-2.0)] = getPoint(theta+1,slices,miniT+1,tess,0);
            m_triangles[n] = new Triangle(normal,points);
            n++;

            normal = new Point*[3];
            points = new Point*[3];

            points[hollow*2] = getPoint(theta,slices,miniT,tess,1);
            normal[hollow*2] = getPoint(theta,slices,miniT,tess,0);

            points[1] = getPoint(theta+1,slices,miniT+1,tess,1);
            normal[1] = getPoint(theta+1,slices,miniT+1,tess,0);

            points[(int)abs(hollow*2-2.0)] = getPoint(theta,slices,miniT+1,tess,1);
            normal[(int)abs(hollow*2-2.0)] = getPoint(theta,slices,miniT+1,tess,0);

            m_triangles[n] = new Triangle(normal,points);
            n++;

            //Psst... you found an achievement! +10 points.
        }
    }

    return m_triangles;
}

Point* Torus::getPoint(int o, int maxo, int i, int maxi, float rmod) {

    float r = max(m_three/200.0, 0.05)+0.6*(1-rmod);
    float R = 0.5 - r;

    Point* p = new Point(((1-rmod)*m_hollow*2-1)*(R*rmod+r*cos(m_PI*2.0*o/maxo))*cos(m_PI*2.0*i/maxi),
                         (((1-rmod)*m_hollow*2-1)*(R*rmod+r*cos(m_PI*2.0*o/maxo))*sin(m_PI*2.0*i/maxi)),
                         (((1-rmod)*m_hollow*2-1)*r*sin(m_PI*2.0*o/maxo))/5.0,1);

    return p;
}
