#include "planetmaster.h"

PlanetMaster::PlanetMaster()
{
    m_planets = QList<Planet*>();
    mid=new Sphere();
    mid->tesselate(50,50);

}

PlanetMaster::~PlanetMaster() {
    //TODO
    // delete m_planets
}

void PlanetMaster::addPlanet(GLuint *texture, double radius, double perturbation) {





   // TODO perturb triangles
    random_vals_t *rv;
    Vector4 p=stochastic::position(rv);
    Vector4 v=stochastic::position(rv);
    int s=rand();
    s=s%50;
    Planet* temp = new Planet(mid->get_triangles(),mid->get_number_of_triangles() );
  //  printf("%d\t%d\t%d\n",rv->p.x,rv->p.y,rv->p.z);


    fflush(stdout);
    temp->set_velocity(v);
    temp->scale(getScaleMat(Vector4(s,s,s,1)));
    temp->trans(getTransMat(p));
    m_planets.append(temp);

}

QList<Planet*> PlanetMaster::getPlanets() {

    return m_planets;

}
