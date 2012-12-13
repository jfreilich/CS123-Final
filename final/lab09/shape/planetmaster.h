#ifndef PLANETMASTER_H
#define PLANETMASTER_H

#define TEXTURES 4
#include <QGLShaderProgram>
#include "shape/planet.h"
#include <QList>


class PlanetMaster
{
public:
    PlanetMaster();
    virtual ~PlanetMaster();
    void remove_planet(int i);
    void addPlanet(Vector3 cam_pos);
    QList<Planet*> getPlanets();

private:
    QList<Planet*> m_planets;
    Sphere* mid;
    GLuint* textures;
};

#endif // PLANETMASTER_H
