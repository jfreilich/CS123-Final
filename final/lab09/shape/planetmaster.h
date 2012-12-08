#ifndef PLANETMASTER_H
#define PLANETMASTER_H


#include <QGLShaderProgram>
#include "shape/planet.h"
#include <QList>


class PlanetMaster
{
public:
    PlanetMaster();
    virtual ~PlanetMaster();
    void remove_planet(int i);
    void addPlanet(GLuint *texture, double radius, double perturbation);
    QList<Planet*> getPlanets();

private:
    QList<Planet*> m_planets;
    Sphere* mid;
    GLuint* textures;
};

#endif // PLANETMASTER_H
