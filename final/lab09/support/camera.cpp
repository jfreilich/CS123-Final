#include "camera.h"
#include <qgl.h>
#include "CS123Algebra.h"
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

Matrix4x4 OrbitCamera::getViewingTransformation() {

    /*Matrix4x4 M2 = Matrix4x4(cotan(getWidthAngle()/2.0)/m_far,0,0,0,
                             0,(cotan(m_heightAngle/2.0))/m_far,0,0,
                             0,0,1.0/m_far,0,
                             0,0,0,1);
    Vector4 u,v,w;

    w = -look;
    u = up.cross(w).getNormalized();
    v = w.cross(u);

    Matrix4x4 M3 = Matrix4x4(u.x,u.y,u.z,0,
                             v.x,v.y,v.z,0,
                             w.x,w.y,w.z,0,
                             0,0,0,1);

    Matrix4x4 M4 = Matrix4x4(1,0,0,(-1.0)*m_position.x,
                             0,1,0,(-1.0)*m_position.y,
                             0,0,1,(-1.0)*m_position.z,
                             0,0,0,1);

    return M4.getInverse()*M3.getInverse()*M2.getInverse();*/

}
