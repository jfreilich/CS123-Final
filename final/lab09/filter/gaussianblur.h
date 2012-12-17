#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H

#include <CS123Common.h>

class GaussianBlur
{
public:
    GaussianBlur(int radius);
    QImage filter(QImage img);
    ~GaussianBlur();
protected:
    QColor blur(int posx, int posy, int num, QImage data, int width, QPoint mtl, QPoint mbr);
    float* m_kernel;
    int m_radius;
};


#endif // GAUSSIANBLUR_H
