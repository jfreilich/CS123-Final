#ifndef CAMERA_H
#define CAMERA_H

#include <QMouseEvent>
#include "math/CS123Algebra.h"
#include <iostream>
using namespace std;

/**
    An orbiting perspective camera specified by a center, two angles, and a zoom factor

    @author: Justin Ardini (jardini)
**/
struct OrbitCamera
{
    Vector3 center, dir, eye, up;
    float theta, phi;
    float fovy;
    float zoom;
    float far;
    float fovx;

    void mouseMove(const Vector2 &delta);
    void move(Vector3 direction, float delta);
    Matrix4x4 getViewingTransformation();
    REAL toRadians(float degrees);
};

#endif // CAMERA_H
