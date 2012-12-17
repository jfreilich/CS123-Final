#include "camera.h"
#include <qgl.h>
#include "CS123Algebra.h"
#include <iostream>
#include "math.h"

#define M_2PI (2.0*M_PI)
void OrbitCamera::mouseMove(const Vector2 &delta)
{
    // Rotate the eye vector around the origin
    theta += delta.x * 0.01f;
    phi -= delta.y * 0.01f;

    // Keep theta in [0, 2pi] and phi in [-pi/2, pi/2]
    theta -= floorf(theta / M_2PI) * M_2PI;
    phi = max(0.01f - M_PI / 2, min(M_PI / 2.0 - 0.01f, (double) phi));

    dir = Vector3(cosf(theta) * cosf(phi), sinf(phi), sinf(theta) * cosf(phi));
}

void OrbitCamera::move(Vector3 direction, float delta)
{
    if (direction.x == 1)
            eye += dir.cross(up) * delta;
    if (direction.x == -1)
            eye -= dir.cross(up) * delta;
    if (direction.y == 1)
            eye += dir * delta;
    if (direction.y == -1)
            eye -= dir * delta;
    if (direction.z == -1)
            eye += (dir.cross(up)).cross(dir) * delta;
    if (direction.z == 1)
            eye -= (dir.cross(up)).cross(dir) * delta;

}

REAL OrbitCamera::toRadians(float degrees)
{
    return degrees * (3.14159265358979/180.0);
}

Matrix4x4 OrbitCamera::getViewingTransformation() {

    Matrix4x4 M2 = Matrix4x4(1.0/(tan(toRadians(fovy/2.0)))/far,0,0,0,
                             0,(1.0/(tan(toRadians(fovx/2.0))))/far,0,0,
                             0,0,1.0/far,0,
                             0,0,0,1);

    Vector3 u,v,w;

    w = -dir;
    u = up.cross(w).getNormalized();
    v = w.cross(u);

    Matrix4x4 M3 = Matrix4x4(u.x,u.y,u.z,0,
                             v.x,v.y,v.z,0,
                             w.x,w.y,w.z,0,
                             0,0,0,1);

    Matrix4x4 M4 = Matrix4x4(1,0,0,(-1.0)*eye.x,
                             0,1,0,(-1.0)*eye.y,
                             0,0,1,(-1.0)*eye.z,
                             0,0,0,1);

    return M4.getInverse()*M3.getInverse()*M2.getInverse();

}
