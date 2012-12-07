#ifndef PLANETMASTER_H
#define PLANETMASTER_H

#include "math/CS123Algebra.h"
#include <QGLShaderProgram>
#include "shape/planet.h"
#include "shape/sphere.h"
#include <QList>

#include <GL/gl.h>


class PlanetMaster
{
public:
    PlanetMaster();
    virtual ~PlanetMaster();

    void addPlanet(GLuint *texture, double radius, double perturbation);
    QList<Planet*> getPlanets();

private:
    QList<Planet*> m_planets;
    Sphere* mid;
    GLuint* textures;
};

#endif // PLANETMASTER_H
