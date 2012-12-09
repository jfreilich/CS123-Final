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
void PlanetMaster::remove_planet(int i){
    delete m_planets.at(i);
    m_planets.removeAt(i);
}

void PlanetMaster::addPlanet() {





   // TODO perturb triangles
    random_vals_t *rv;
    Vector4 p=stochastic::position(rv);
    Vector4 v=stochastic::velocity(rv)/1000.0;
    int r=rand();
    r=r%101;
    double rd=r/100.0f;
    rd=rd*M_PI/16.0;
    int r2=rand();
    r2=r2%101;
    double ra=r/100.0f;
    ra*=M_PI/2.0;
    int orbit=rand();
    orbit=(orbit)%101;
    double orbitd=orbit/100.0f;
    orbitd*=M_PI/16.0;
    int s=rand();
    s=s%50;

    int texture=rand();
    texture=texture%TEXTURES;
    GLUquadric* sphere=gluNewQuadric();
    
    Planet* temp = new Planet(mid->get_triangles(),mid->get_number_of_triangles(),sphere );

    temp->set_velocity(v);
    temp->scale(getScaleMat(Vector4(s,s,s,1)));
    temp->set_radius(s);
    temp->trans(getTransMat(p));
    temp->set_axis_angle(ra);
    Matrix4x4 rot=getRotXMat(rd);
    temp->rot(rot);
    temp->orbit_rot(getRotZMat(orbitd));
    temp->calculate_composite_transformations();
    temp->set_texture(texture);
    m_planets.append(temp);

}

QList<Planet*> PlanetMaster::getPlanets() {

    return m_planets;

}
