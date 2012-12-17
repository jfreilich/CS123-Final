#include "gaussianblur.h"

#include <iostream>

using namespace std;

GaussianBlur::GaussianBlur(int radius)
{


    m_radius = radius;

    m_kernel = new float[m_radius*2+1];

    //float x = 0.3333333333;//pow(2,m_radius)/(3*pow(2,m_radius)-2);
    //float x = (m_radius==1) ? 0.5 : 2.0/(2*(m_radius-1)+1);

    float pi = 3.14159265358979;
    float o = m_radius/3.0;

    float sum = 0;
    float in;
    for (int i=0;i<=m_radius;i++) {
        in = exp(0-i*i/(2*o*o))*1.0/sqrt(2*pi*o*o);
        m_kernel[m_radius-i] = in;
        m_kernel[m_radius+i] = in;
        sum += (i==0 ? 1 : 2) * in;
    }
    for (int i=0;i<=m_radius*2;i++) {
        m_kernel[i] = m_kernel[i]/sum;
    }
}

GaussianBlur::~GaussianBlur()
{
    delete[] m_kernel;
}
QImage GaussianBlur::filter(QImage img) {

    int width = img.width();
    int height = img.height();

    QImage towrite = QImage(width,height,QImage::Format_ARGB32);
    QImage towrite2 = QImage(width,height,QImage::Format_ARGB32);

    QPoint tl = QPoint(0,0);
    QPoint br = QPoint(width,height);

    // filter x-wise
    for (int j=0;j<height;j++) {
        for (int i=0;i<width;i++) {
            towrite.setPixel(i,j,blur(i+tl.x(),j+tl.y(),0,img,width,tl,br).rgb());
        }
    }
    // blur x-wise
    for (int j=0;j<height;j++) {
        for (int i=0;i<width;i++) {
            towrite2.setPixel(i,j,
                              blur(i*1,j*1,1,towrite,width,QPoint(0,0),QPoint(width,height)).rgb());
        }
    }

    return towrite2;
}

QColor GaussianBlur::blur(int posx, int posy, int dir, QImage data, int width, QPoint mtl, QPoint mbr) {

    float r = 0,b = 0,g = 0;
    int x,y;

    for (int i=0;i<m_radius*2+1;i++) {
        x = min(mbr.x()-1,max(mtl.x()+1,posx+dir*(i-m_radius)));
        y = min(mbr.y()-1,max(mtl.y()+1,posy+(1-dir)*(i-m_radius)));
        QRgb color = data.pixel(x,y);
        r += m_kernel[i]*qRed(color);
        g += m_kernel[i]*qGreen(color);
        b += m_kernel[i]*qBlue(color);
    }

    QColor c = QColor((int)r,(int)g,(int)b);

    return c;
}
