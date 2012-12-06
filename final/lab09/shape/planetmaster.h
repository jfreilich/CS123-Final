#ifndef PLANETMASTER_H
#define PLANETMASTER_H

#include "math/vector.h"
#include <QGLShaderProgram>
#include "jasonShapes/sphere.h"
#include "shape/planet.h"
#include <QList>


class PlanetMaster
{
public:
    PlanetMaster();
    virtual ~PlanetMaster();

    void addPlanet(GLuint *texture, double radius, double perturbation);
    QList<Planet*> getPlanets();

private:
    QList<Planet*> m_planets;
    Sphere sphereGen;
};

#endif // PLANETMASTER_H
