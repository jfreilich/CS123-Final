#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "CS123Algebra.h"
#include "shape/planet.h"

class RayTracer
{
public:
    struct Closest {
            double t;
            Planet *planet;
            Vector4 normal;
            Vector4 intersectObjectSpace;
            Vector4 intersect;
    };

    static Vector4 generateRayD(double x, double y, int width, int height, Vector4 eye,  Matrix4x4 inv);
    static double getTValue(Planet* o, Vector4 invP, Vector4 objectD, Vector4 &normal);
    static RayTracer::Closest* performTrace(Vector4 towardsSurface, Vector4 pos,  QList<Planet*> planets);

};

#endif // RAYTRACER_H
