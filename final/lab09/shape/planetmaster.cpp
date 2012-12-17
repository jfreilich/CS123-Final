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

void PlanetMaster::create_solar_system(){
    Vector4 v,p;
    double s,ra, rd, mass,texture;

    Matrix4x4 rot;
    //make a sun
    Planet* sun=new Planet();
    s=400;
    ra=0.0;
    rd=0.01;
    v=Vector4(0,0,0,0);
    texture=15;
    p=Vector4(0.0,0.0,0.0,1.0);
    mass=100000000.0;
    Planet * temp=sun;
    temp->set_velocity(v);
    temp->set_scale(getScaleMat(Vector4(s,s,s,1)));
    temp->set_radius(s);
    temp->set_trans(getTransMat(p));
    temp->set_axis_angle(ra);
    temp->set_mass(mass);
    rot=getRotXMat(rd);
    temp->set_rot(rot);
    temp->calculate_composite_transformations();
    temp->set_texture(texture);
    m_planets.append(temp);



    Planet* earth=new Planet();
    mass=1.0;
    temp=earth;
    s=16;
    texture=10;
    p=Vector4(512.0,0.0,0.0,1.0);
    v=Vector4(0.0,0.0,50.0,0.0);
    temp->set_velocity(v);
    temp->set_scale(getScaleMat(Vector4(s,s,s,1)));
    temp->set_radius(s);
    temp->set_trans(getTransMat(p));
    temp->set_axis_angle(ra);
    temp->set_mass(mass);
    rot=getRotXMat(rd);
    temp->set_rot(rot);
    temp->calculate_composite_transformations();
    temp->set_texture(texture);
    m_planets.append(temp);


    Planet* jupiter=new Planet();
    mass=317.8;
    temp=jupiter;
    s=96;
    texture=14;
    p=Vector4(-1024.0,0.0,0.0,1.0);
    v=Vector4(0.0,0.0,-2.0,0.0);
    temp->set_velocity(v);
    temp->set_scale(getScaleMat(Vector4(s,s,s,1)));
    temp->set_radius(s);
    temp->set_trans(getTransMat(p));
    temp->set_axis_angle(ra);
    temp->set_mass(mass);
    rot=getRotXMat(rd);
    temp->set_rot(rot);
    temp->calculate_composite_transformations();
    temp->set_texture(texture);
    m_planets.append(temp);



}

void PlanetMaster::addPlanet(Vector3 cam_pos) {


   // TODO perturb triangles
    random_vals_t *rv;
    Vector4 p=stochastic::position(rv,cam_pos);
    Vector4 v=stochastic::velocity(rv)/32.0;
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
    s=s%32;
    s+=64;




    double density=(rand()%1000)/999.0;
    density=1.0;

    int texture=rand();
    texture=texture%13;
    texture+=3;

//    if (texture==1||texture==2){
//        texture=3;
//    }

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
