#include "raytracer.h"


static Vector4 RayTracer::generateRayD(double x, double y, int width, int height, Vector4 eye, Matrix4x4 m) {

    Vector4 pFilm = Vector4(2.0*(x)/(1.0*width)-1, 1 - 2.0*(y)/(1.0*height), -1, 0);

    Vector4 d = (m*pFilm - eye).getNormalized();

    return d;
}

static double RayTracer::getTValue(Planet* o, Vector4 invP, Vector4 objectD, Vector4 &normal) {

    double a, b, c, t = NULL, tempt = NULL;
    Vector4 plane;

    a = objectD.x*objectD.x + objectD.y*objectD.y + objectD.z*objectD.z;
    b = 2*(objectD.x*invP.x + objectD.y*invP.y + objectD.z*invP.z);
    c = invP.x*invP.x + invP.y*invP.y + invP.z*invP.z - 0.5*0.5;

    if (b*b - 4*a*c < 0)
        return NULL; // no roots, so move on

    float t1 = (-b + sqrt(b*b - 4 * a * c))/(2*a);
    float t2 = (-b - sqrt(b*b - 4 * a * c))/(2*a);
    t = min(t1,t2);

    Vector4 intersection = (invP + objectD*t);
    intersection.w = 0;

    normal = intersection.getNormalized();

    return (t == NULL) ? NULL : max(t,0.0);
}

static RayTracer::Closest* RayTracer::performTrace(Vector4 towardsSurface, Vector4 pos, QList<Planet*> planets) {

    Closest *closest = new Closest();
    closest->t = 0;
    closest->planet = NULL;
    closest->normal = Vector4::zero();
    closest->intersectObjectSpace = Vector4::zero();
    closest->intersect = Vector4::zero();

    for (int i=0;i<planets.size();i++) {
        Matrix4x4 inverseTrans = planets.at(i)->get_trans().getInverse();

        //world to object space
        // use t in world space for lighting etc.
        Vector4 invP = inverseTrans * pos;
        Vector4 objectD = inverseTrans * towardsSurface;
        Vector4 daNormal;

        double t = getTValue(planets.at(i), invP, objectD, daNormal);

        if (t < 0.0001 || t == NULL || (closest->planet != NULL && closest->t <= t))
            continue;

        closest->t = t;

        closest->planet = planets.at(i);

        closest->intersect = pos + towardsSurface*t;
        closest->intersectObjectSpace = invP + objectD*t;
        Matrix4x4 trans3 = planets.at(i)->get_trans();
        trans3.d = 0;
        trans3.h = 0;
        trans3.l = 0;
        trans3.p = 1;
        trans3.m = 0;
        trans3.n = 0;
        trans3.o = 0;
        trans3 = trans3.getInverse();//.getTranspose()
        closest->normal = trans3*daNormal;
    }

    return closest;

}
