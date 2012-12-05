#ifndef VIEW_H
#define VIEW_H

#include <qgl.h>
#include <QTime>
#include <QTimer>
#include "support/camera.h"

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QTime time;
    QTimer timer;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void initializeResources();
    void loadCubeMap();

    void applyOrthogonalCamera(float width, float height);
    void applyPerspectiveCamera(float width, float height);
    void paintText();

    int m_prevTime;
    float m_prevFps, m_fps;

    GLuint m_skybox;
    GLuint m_cubeMap;

    OrbitCamera m_camera;
    QFont m_font; // font for rendering text


private slots:
    void tick();
};

#endif // VIEW_H

