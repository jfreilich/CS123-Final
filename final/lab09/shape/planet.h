#ifndef PLANET_H
#define PLANET_H

#include "math/CS123Algebra.h"
#include <iostream>
#include "shape/sphere.h"
#include <GL/gl.h>
#include "math/stochastic.h"
#include <GL/glu.h>
using namespace std;

class Planet
{
public:
    Planet(triangle_t* mid, int numMid,GLUquadric* sphere );
    virtual ~Planet();

    void modifyColor(Vector4 baseR, Vector4 baseG, Vector4 baseB);
    triangle_t* getMid(int &num);
    GLuint getTexture();
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
    void move();
    void set_axis_angle(double angle);
    void calculate_composite_transformations();
    void orbit_rot(Matrix4x4 rot);
    Matrix4x4 get_trans();
    GLUquadric* get_sphere();
   double get_radius();
    void set_texture(int texture);
    void set_radius(double r);

private:
    triangle_t* m_mid;
    int m_numMid;
    GLUquadric* m_sphere;
    int m_texture;
    Vector4 m_r, m_g, m_b, m_p, m_v;
    Matrix4x4 m_axis, m_rot, m_rot_velocity, m_orbit_rot, m_orbit_rot_velocity; //rotational motion
    Matrix4x4 m_trans, m_velocity; //linear motion
    Matrix4x4 m_scale, m_constant; //constants
    Matrix4x4 m_total;
    double m_axis_angle,m_radius;

};

#endif // PLANET_H
