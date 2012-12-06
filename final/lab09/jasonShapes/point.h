#ifndef POINT_H
#define POINT_H

class Point {
public:
    Point(float x1, float y1, float z1, float n1);
    float x;
    float y;
    float z;
    float n;
    void switchXY();
    void switchXZ();
    void switchYZ();
};

#endif // POINT_H
