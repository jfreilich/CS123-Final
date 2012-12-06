#include "planetmaster.h"

PlanetMaster::PlanetMaster()
{
    m_planets = QList<Planet*>();
    sphereGen = Sphere();

}

PlanetMaster::~PlanetMaster() {
    //TODO
    // delete m_planets
}

void PlanetMaster::addPlanet(GLuint *texture, double radius, double perturbation) {

    int numtris = 0;

    Triangle** triangles = sphereGen.generate(49,49, numtris);

   // TODO perturb triangles

    Planet* temp = new Planet(triangles, numtris);

    m_planets.append(temp);

}

QList<Planet*> PlanetMaster::getPlanets() {

    return m_planets;

}
