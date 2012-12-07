#include "Shape.h"
#include "math.h"
Shape::Shape()
{
     m_triangles=NULL;
     m_size=0;
}
Shape::~Shape(){
    if(m_triangles){
        delete[] m_triangles;
    }
}
int Shape::get_number_of_triangles(){
    return m_size;
}
void Shape::tesselate(int p1, int p2){

}
triangle_t* Shape::get_triangles(){
    return m_triangles;
}

triangle Shape::get_triangle(int index){
    if (index<=m_size){
        return m_triangles[index];
    }
    else{
        triangle tri;
        return tri;
    }
}

void Shape::normal_vector(double points[9], double normal[9]){

}
