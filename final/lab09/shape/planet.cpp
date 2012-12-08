#include "planet.h"
#include <cstring>
Planet::Planet(triangle_t* mid, int numMid,GLUquadric* sphere)
{
    m_mid =new triangle_t[numMid];
    m_numMid = numMid;
    memcpy(m_mid, mid, numMid*sizeof(triangle_t));
    m_trans=Matrix4x4::identity();
    m_rot=Matrix4x4::identity();
    m_scale=Matrix4x4::identity();
    m_constant=Matrix4x4::identity();
    m_axis=Matrix4x4::identity();
    m_orbit_rot=Matrix4x4::identity();
    m_total=Matrix4x4::identity();
    m_sphere=sphere;





}

triangle_t* Planet::getMid(int &num) {

    num = m_numMid;
    return m_mid;

}
void Planet::set_velocity(Vector4 v){
    m_v=v;
    m_velocity=getTransMat(m_v);
}

Vector4 Planet::getV(){
    return m_v;
}

Planet::~Planet(){
//    if (m_low){
//        delete[] m_low;
//    }
    if (m_mid){
        delete[] m_mid;
    }
    delete m_sphere;
//    if (m_high){
//        delete[] m_high;
//    }
}





void Planet::set_axis_angle(double angle){
    m_axis_angle=angle;
    m_axis=getRotYMat(m_axis_angle);
}


GLuint Planet::getTexture(){
    return m_texture;
}

Vector4 Planet::getR(){
    return m_r;
}

Vector4 Planet::getG(){
    return m_g;
}

Vector4 Planet::getB(){
    return m_b;
}

Matrix4x4 Planet::getRot(){
    return m_rot;
}

Matrix4x4 Planet::getTrans(){
    return m_trans;
}

Matrix4x4 Planet::getScale(){
    return m_scale;
}

void Planet::trans(Matrix4x4 trans){
    m_trans=trans;
}
Matrix4x4 Planet::get_trans(){
    return m_trans;
}
double Planet::get_radius(){
    return m_radius;
}

void Planet::set_radius(double r){
    m_radius=r;
}

Matrix4x4 Planet::getTotalTrans(){
    return m_total;
}

void Planet::scale(Matrix4x4 scale){
    m_scale=scale;
}

void Planet::calculate_composite_transformations(){
   // m_constant=m_scale;
    m_total=m_orbit_rot*m_trans*m_rot*m_constant;
}

void Planet::rot(Matrix4x4 rot){
    m_rot_velocity=rot;
}

GLUquadric* Planet::get_sphere(){
    return m_sphere;
}

void Planet::move(){
    m_trans *= m_velocity;
    m_rot *= m_rot_velocity;
    m_orbit_rot *= m_orbit_rot_velocity;
    m_total = m_trans * m_axis * m_rot * m_constant;
}

void Planet::orbit_rot(Matrix4x4 rot){
    m_orbit_rot_velocity=rot;
}
