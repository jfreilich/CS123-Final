#ifndef PLANET_H
#define PLANET_H

#include "math/CS123Algebra.h"
#include <iostream>
#include "shape/sphere.h"
#include <GL/gl.h>
#include "math/stochastic.h"
#include <GL/glu.h>
#include "support/particleemitter.h"

using namespace std;

class Planet
{
public:
    Planet();
    virtual ~Planet();
    GLuint get_texture();
    double get_radius();
    double get_radius3();
    double get_density();
    double get_mass();
    Vector4 get_r();
    Vector4 get_g();
    Vector4 get_b();
    Vector4 get_velocity();
    Vector4 get_position();
    Matrix4x4 get_rot();
    Matrix4x4 get_trans();
    Matrix4x4 get_scale();
    Matrix4x4 get_total_trans();
    void set_density(double density);
    void set_mass(double density);
    void set_radius3(double r3);
    void set_trans(Matrix4x4 trans);
    void set_scale(Matrix4x4 scale);
    void set_rot(Matrix4x4 rot);
    void set_velocity(Vector4 v);
    void set_axis_angle(double angle);
    void set_orbit_rot(Matrix4x4 rot);
    void set_texture(int texture);
    void set_radius(double r);
    void move();
    void calculate_composite_transformations();
    void modify_color(Vector4 baseR, Vector4 baseG, Vector4 baseB);
    //public variables? Jason how could you...
    ParticleEmitter *emitter;

private:
 //   GLUquadric* m_sphere;
    int m_texture;
    double m_density, m_mass;
    Vector4 m_position;
    Vector4 m_r, m_g, m_b, m_p, m_v;
    Matrix4x4 m_axis, m_rot, m_rot_velocity, m_orbit_rot, m_orbit_rot_velocity; //rotational motion
    Matrix4x4 m_trans, m_velocity; //linear motion
    Matrix4x4 m_scale, m_constant; //constants
    Matrix4x4 m_total;
    double m_axis_angle ,m_radius, m_radius3;


};

#endif // PLANET_H
