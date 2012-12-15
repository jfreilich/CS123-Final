#include "planetmaster.h"

PlanetMaster::PlanetMaster()
{
    m_planets = QList<Planet*>();
}

PlanetMaster::~PlanetMaster() {
    //TODO
    // delete m_planets
}
void PlanetMaster::remove_planet(int i){
    delete m_planets.at(i);
    m_planets.removeAt(i);
}

void PlanetMaster::addPlanet(Vector3 cam_pos) {


   // TODO perturb triangles
    random_vals_t *rv;
    Vector4 p=stochastic::position(rv,cam_pos);
    Vector4 v=stochastic::velocity(rv)/1000.0;
    int r=rand();
    r=r%101;
    double rd=r/100.0f;
    rd=rd*M_PI/128.0;
    int r2=rand();
    r2=r2%101;
    double ra=r/100.0f;
    ra*=M_PI/2.0;
    int orbit=rand();
    orbit=(orbit)%101;
    double orbitd=orbit/100.0f;
    orbitd*=M_PI/16.0;
    int s=rand();
    s=s%6;
    s*=100;
    s+=500;



    double density=(rand()%1000)/999.0;
    density=1.0;
//    if (s>=45){
//        s*=3;
//    }

    int texture=rand();
    texture=texture%TEXTURES;
//    if (texture<5){
//        texture=0;
//    }
//    else if(texture<10){
//        texture=1;
//    }
//    else{
//        texture=2;
//    }
    if (texture==1){
        texture++;
    }
    texture=7;
    Planet* temp = new Planet();

    temp->set_velocity(v);
    temp->set_scale(getScaleMat(Vector4(s,s,s,1)));
    temp->set_radius(s);
    temp->set_trans(getTransMat(p));
    temp->set_axis_angle(ra);
    temp->set_density(density);
    Matrix4x4 rot=getRotXMat(rd);
    temp->set_rot(rot);
    temp->set_orbit_rot(getRotZMat(orbitd));
    temp->calculate_composite_transformations();
    temp->set_texture(texture);
    m_planets.append(temp);

}

QList<Planet*> PlanetMaster::getPlanets() {

    return m_planets;

}
