#include "planet.h"
#include <cstring>

Planet::Planet()
{

    m_trans=Matrix4x4::identity();
    m_rot=Matrix4x4::identity();
    m_scale=Matrix4x4::identity();
    m_constant=Matrix4x4::identity();
    m_axis=Matrix4x4::identity();
    m_orbit_rot=Matrix4x4::identity();
    m_total=Matrix4x4::identity();
    m_density=0.0;
    m_mass=0.0;
    m_radius=0;
    m_radius3=0;
    m_position=Vector4(0.0,0.0,0.0,1.0);
    emitter = new ParticleEmitter(2);

}
void Planet::set_texture(int texture){
    m_texture=texture;
}
Vector4 Planet::get_position(){
    return m_position;
}

void Planet::set_velocity(Vector4 v){
    m_v=v;
    m_velocity=getTransMat(m_v);
}

Vector4 Planet::get_velocity(){
    return m_v;
}

Planet::~Planet(){
    delete emitter;
}
double Planet::get_density(){
    return m_density;
}
double Planet::get_mass(){
    return m_mass;
}
double Planet::get_radius3(){
    return m_radius3;
}
double Planet::get_radius(){
    return m_radius;
}
void Planet::set_density(double density){
    m_density=density;
    m_mass=m_radius3*density;
}
void Planet::set_mass(double mass){
    m_mass=mass;
    m_density=mass/m_radius3;
}
void Planet::set_radius(double r){
    m_radius=r;
    m_radius3=r*r*r;
    m_mass=m_density*m_radius3;
}
void Planet::set_radius3(double r3){
    m_radius3=r3;
    m_radius=pow(r3,1.0/3.0);
    m_mass=m_density*r3;
}
void Planet::set_axis_angle(double angle){
    m_axis_angle=angle;
    m_axis=getRotYMat(m_axis_angle);
}
GLuint Planet::get_texture(){
    return m_texture;
}

Vector4 Planet::get_r(){
    return m_r;
}

Vector4 Planet::get_g(){
    return m_g;
}

Vector4 Planet::get_b(){
    return m_b;
}

Matrix4x4 Planet::get_rot(){
    return m_rot;
}

Matrix4x4 Planet::get_trans(){
    return m_trans;
}

Matrix4x4 Planet::get_scale(){
    return m_scale;
}

void Planet::set_trans(Matrix4x4 trans){
    m_trans=trans;
    m_position=Vector4(trans.d,trans.h,trans.l,1.0);
}

Matrix4x4 Planet::get_total_trans(){
    return m_total;
}

void Planet::set_scale(Matrix4x4 scale){
    m_scale=scale;
}

void Planet::calculate_composite_transformations(){
   // m_constant=m_scale;
    m_total=m_orbit_rot*m_trans*m_rot*m_constant;
}

void Planet::set_rot(Matrix4x4 rot){
    m_rot_velocity=rot;
}


void Planet::move(){
    m_trans *= m_velocity;
    m_position+=m_v;
    m_rot *= m_rot_velocity;
    m_orbit_rot *= m_orbit_rot_velocity;
    m_total =m_trans * m_axis * m_rot * m_constant;
}

void Planet::set_orbit_rot(Matrix4x4 rot){
    m_orbit_rot_velocity=rot;
}
