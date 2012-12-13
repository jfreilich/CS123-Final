#include "camera.h"
#include <qgl.h>

#include <iostream>

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

void OrbitCamera::move(Vector2 direction, float delta)
{
    if (direction.x == 1)
            eye += dir.cross(up) * delta;
    if (direction.x == -1)
            eye -= dir.cross(up) * delta;
    if (direction.y == 1)
            eye += dir * delta;
    if (direction.y == -1)
            eye -= dir * delta;

    //eye.y = max(-290.0,min(290.0,eye.y));
    //eye.x = max(-290.0,min(290.0,eye.x));
    //eye.z = max(-290.0,min(290.0,eye.z));

    //cout << eye << endl;
}
