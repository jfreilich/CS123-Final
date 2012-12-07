#ifndef SHAPE_H
#define SHAPE_H
#include <stdio.h>
#include <math.h>
typedef struct triangle{
    double coordinates[9];
    double normals[9];
}triangle_t;

class Shape {
public:
    Shape();
    virtual ~Shape();
    struct triangle get_triangle(int index);
    triangle_t* get_triangles();
    int get_number_of_triangles();
    virtual void tesselate(int p1, int p2);
protected:
    int m_size;
    virtual void normal_vector(double points[9], double normal[9]);
    struct triangle *m_triangles;
};

#endif // SHAPE_H
