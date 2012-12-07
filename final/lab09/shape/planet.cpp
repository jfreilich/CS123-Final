#include "planet.h"
#include <cstring>
Planet::Planet(triangle_t* mid, int numMid)
{
    m_mid =new triangle_t[numMid];
    m_numMid = numMid;
    memcpy(m_mid, mid, numMid*sizeof(triangle_t));
    m_trans=Matrix4x4::identity();
    m_rot=Matrix4x4::identity();
    m_scale=Matrix4x4::identity();


    /*std::copy(low, low[lowsize], m_low);
    std::copy(mid, mid[midsize], m_mid);
    std::copy(high, high[highsize], m_high);*/

}

triangle_t* Planet::getMid(int &num) {

    num = m_numMid;
    return m_mid;

}
void Planet::set_velocity(Vector4 v){
    m_v=v;
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
//    if (m_high){
//        delete[] m_high;
//    }
}


unsigned int Planet::getTexture(){
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

Matrix4x4 Planet::getTotalTrans(){
     m_total=m_trans*m_rot*m_scale;
    return m_total;
}

void Planet::trans(Matrix4x4 trans){
    m_trans=trans*m_trans;
}

void Planet::scale(Matrix4x4 scale){
    m_scale=scale*m_scale;
}

void Planet::rot(Matrix4x4 rot){
    m_rot=rot*m_rot;
}
