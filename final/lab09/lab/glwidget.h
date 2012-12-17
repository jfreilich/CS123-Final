#ifndef GLWIDGET_H
#define GLWIDGET_H

#define SKYBOX_RADIUS 4096
#include <QGLWidget>
#include <QHash>
#include <QString>
#include <QTimer>
#include <QTime>
#include <math.h>
#include "camera.h"
#include "resourceloader.h"
#include "shape/planetmaster.h"
#include <GL/glu.h>
class QGLShaderProgram;
class QGLFramebufferObject;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    // Overridden QGLWidget methods
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    // Initialization code
    void initializeResources();
    void loadCubeMap();
    void createShaderPrograms();
    void createFramebufferObjects(int width, int height);
    void createBlurKernel(int radius, int width, int height, GLfloat* kernel, GLfloat* offsets);

    // Drawing code
    void applyOrthogonalCamera(float width, float height);
    void applyPerspectiveCamera(float width, float height);
    void renderTexturedQuad(int width, int height);
    void renderBlur(int w, int h, Planet* p, int n);
    void renderScene();
    void paintText();
    GLuint LoadTextureRAW( const char * filename, int wrap );
    GLuint loadTexture(const QString &filename);
    void create_solar_system();
    void adjustSize();

    Planet* findClosestPlanet();

    void handleKeys();

protected slots:
    void tick();

private:
    QTimer m_timer;
    QTime m_clock;

    QList<ParticleEmitter*> m_emitters;
    QList<Matrix4x4> m_emitterTrans;

    Planet* revolving;

    void revolveCamera();

    QList<int> keys;
    int m_prevTime;
    float m_prevFps, m_fps;
    Vector2 m_prevMousePos;
    OrbitCamera m_camera;
    bool pause;
    bool solar;
    GLUquadric* m_glu_sphere;

    // Resources
    QHash<QString, QGLShaderProgram *> m_shaderPrograms; // hash map of all shader programs
    QHash<QString, QGLFramebufferObject *> m_framebufferObjects; // hash map of all framebuffer objects
    GLuint* m_textures;// list of textures
    Vector4* m_texture_colors;//list of static color of texture
    PlanetMaster m_pms; // master of planets
    GLuint m_skybox; // skybox call list ID
    GLuint m_cubeMap; // cubeMap texture ID
    QFont m_font;
};

#endif // GLWIDGET_H
