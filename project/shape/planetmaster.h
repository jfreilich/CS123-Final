#ifndef PLANETMASTER_H
#define PLANETMASTER_H

#define TEXTURES 16
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
    void create_solar_system();

private:
    QList<Planet*> m_planets;
    GLuint* textures;
};

#endif // PLANETMASTER_H
