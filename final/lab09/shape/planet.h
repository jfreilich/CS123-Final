#ifndef PLANET_H
#define PLANET_H

#include "math/CS123Algebra.h"
#include <iostream>
#include "shape/sphere.h"
#include <GL/gl.h>
#include "math/stochastic.h"

using namespace std;

class Planet
{
public:
    Planet(triangle_t* mid, int numMid );
    virtual ~Planet();

    void modifyColor(Vector4 baseR, Vector4 baseG, Vector4 baseB);
    triangle_t* getMid(int &num);
    unsigned int getTexture();
    Vector4 getR();
    Vector4 getG();
    Vector4 getB();
    Matrix4x4 getRot();
    Matrix4x4 getTrans();
    Matrix4x4 getScale();
    Matrix4x4 getTotalTrans();
    Vector4 getV();
    void trans(Matrix4x4 trans);
    void scale(Matrix4x4 scale);
    void rot(Matrix4x4 rot);
    void set_velocity(Vector4 v);
private:
    triangle_t* m_mid;
    int m_numMid;
    GLuint m_texture;
    Vector4 m_r, m_g, m_b, m_p, m_v;
    Matrix4x4 m_rot, m_trans , m_scale, m_total;
};

#endif // PLANET_H
