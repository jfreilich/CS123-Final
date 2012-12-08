#include "glwidget.h"

#include <iostream>
#include <QFileDialog>
#include <QGLFramebufferObject>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QTime>
#include <QTimer>
#include <QWheelEvent>
#include "glm.h"

using std::cout;
using std::endl;

extern "C"
{
    extern void APIENTRY glActiveTexture(GLenum);
}

static const int MAX_FPS = 120;

/**
  Constructor.  Initialize all member variables here.
 **/
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent),
    m_timer(this), m_prevTime(0), m_prevFps(0.f), m_fps(0.f),
    m_font("Deja Vu Sans Mono", 8, 4)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    m_camera.center = Vector3(0.f, 0.f, 0.f);
    m_camera.up = Vector3(0.f, 1.f, 0.f);
    m_camera.zoom = 3.5f;
    m_camera.theta = M_PI * 1.5f, m_camera.phi = 0.2f;
    m_camera.fovy = 60.f;


    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

/**
  Destructor.  Delete any 'new'ed objects here.
 **/
GLWidget::~GLWidget()
{
    foreach (QGLShaderProgram *sp, m_shaderPrograms)
        delete sp;
    foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
        delete fbo;
    glDeleteLists(m_skybox, 1);
    const_cast<QGLContext *>(context())->deleteTexture(m_cubeMap);
    //glmDelete(m_dragon.model);

    for (int i=0;i<TEXTURES;i++){
       // glDeleteTextures(1,&(m_textures[i]));
    }
}

/**
  Our loop.
**/
void GLWidget::tick()
{
    update();

    QList<Planet*> planets = m_pms.getPlanets();
    int size = planets.size();
    for (int i=0;i<size;i++) {

        Planet* p = planets.at(i);

        p->move();
        Matrix4x4 trans=p->get_trans();
        float x,y,z;
        x=fabs(trans.d);
        y=fabs(trans.h);
        z=fabs(trans.l);
      //  printf("%i\t%f\t%f\t%f\n",size,x,y,z);
        fflush(stdout);
        double max=300;
        if (x>=max||y>=max||z>=max){
            m_pms.remove_planet(i);
            planets.removeAt(i);
            i--;
            size--;
        }

    }

    int r = rand();
    r=r%100;
    if (r<=5){
        m_pms.addPlanet();
    }
}

GLuint GLWidget::loadTexture(const QString &filename)
{
    // Make sure the image file exists
    fflush(stdout);

    QFile file(filename);
    if (!file.exists())
        return -1;

    // Load the file into memory
    QImage image;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    QImage texture = QGLWidget::convertToGLFormat(image);

    // Generate a new OpenGL texture ID to put our image into
    GLuint id = 0;
    glGenTextures(1, &id);

    // Make the texture we just created the new active texture
    glBindTexture(GL_TEXTURE_2D, id);

    // Copy the image data into the OpenGL texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}




/**
  Initialize the OpenGL state and start the drawing loop.
 **/
void GLWidget::initializeGL()
{
    m_textures=(GLuint *) malloc(10*sizeof(GLuint));
    // Set up OpenGL
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glDisable(GL_DITHER);

    glDisable(GL_LIGHTING);
    glShadeModel(GL_FLAT);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glActiveTexture(GL_TEXTURE0);

   // GLuint t=this->loadTexture("textures/desert");

    GLuint t=this->loadTexture("textures/earth.png");
    if (t==-1){
        cout << "earth texture failed." << endl;
    }
    else{
        m_textures[0]=t;
    }
    t=this->loadTexture("textures/desert");
    if (t==-1){
        cout << "desert texture failed." << endl;
    }
    else{
        m_textures[1]=t;
    }
    t=this->loadTexture("textures/cloud.jpg");
    if (t==-1){
        cout << "cloud texture failed." << endl;
    }
    else{
        m_textures[2]=t;
    }
    t=this->loadTexture("textures/green.jpg");
    if (t==-1){
        cout << "green texture failed." << endl;
    }
    else{
        m_textures[3]=t;
    }
    t=this->loadTexture("textures/jupiter.jpg");
    if (t==-1){
        cout << "jupiter texture failed." << endl;
    }
    else{
        m_textures[4]=t;
    }





    // Load resources, including creating shader programs and framebuffer objects
    initializeResources();

    // Start the drawing timer
    m_timer.start(1000.0f / MAX_FPS);
}

/**
  Initialize all resources.
  This includes models, textures, call lists, shader programs, and framebuffer objects.
 **/
void GLWidget::initializeResources()
{
    cout << "Using OpenGL Version " << glGetString(GL_VERSION) << endl << endl;
    // Ideally we would now check to make sure all the OGL functions we use are supported
    // by the video card.  But that's a pain to do so we're not going to.
    cout << "--- Loading Resources ---" << endl;

    //m_dragon = ResourceLoader::loadObjModel("/course/cs123/bin/models/xyzrgb_dragon.obj");

    //load textures here
    m_pms = PlanetMaster();
    m_pms.addPlanet();
    m_pms.addPlanet();m_pms.addPlanet();m_pms.addPlanet();m_pms.addPlanet();
    cout << "Loaded planet master..." << endl;

    m_skybox = ResourceLoader::loadSkybox();
    cout << "Loaded skybox..." << endl;

    loadCubeMap();
    cout << "Loaded cube map..." << endl;

    createShaderPrograms();
    cout << "Loaded shader programs..." << endl;

    createFramebufferObjects(width(), height());
    cout << "Loaded framebuffer objects..." << endl;

    cout << " --- Finish Loading Resources ---" << endl;
    
}

/**
  Load a cube map for the skybox
 **/
void GLWidget::loadCubeMap()
{
    QList<QFile *> fileList;
    fileList.append(new QFile("stars/starssmall.jpg"));
    fileList.append(new QFile("stars/starssmall.jpg"));
    fileList.append(new QFile("stars/starssmall.jpg"));
    fileList.append(new QFile("stars/starssmall.jpg"));
    fileList.append(new QFile("stars/starssmall.jpg"));
    fileList.append(new QFile("stars/starssmall.jpg"));
    m_cubeMap = ResourceLoader::loadCubeMap(fileList);
}

/**
  Create shader programs.
 **/
void GLWidget::createShaderPrograms()
{
    const QGLContext *ctx = context();
    m_shaderPrograms["planetShader"] = ResourceLoader::newFragShaderProgram(ctx, "shaders/planet.frag");
    m_shaderPrograms["reflect"] = ResourceLoader::newShaderProgram(ctx, "shaders/reflect.vert", "shaders/reflect.frag");
    m_shaderPrograms["refract"] = ResourceLoader::newShaderProgram(ctx, "shaders/refract.vert", "shaders/refract.frag");
    m_shaderPrograms["brightpass"] = ResourceLoader::newFragShaderProgram(ctx, "shaders/brightpass.frag");
    m_shaderPrograms["blur"] = ResourceLoader::newFragShaderProgram(ctx, "shaders/blur.frag");
}

/**
  Allocate framebuffer objects.

  @param width: the viewport width
  @param height: the viewport height
 **/
void GLWidget::createFramebufferObjects(int width, int height)
{
    // Allocate the main framebuffer object for Sing the scene to
    // This needs a depth attachment
    m_framebufferObjects["fbo_0"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::Depth,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    m_framebufferObjects["fbo_0"]->format().setSamples(16);
    // Allocate the secondary framebuffer obejcts for rendering textures to (post process effects)
    // These do not require depth attachments
    m_framebufferObjects["fbo_1"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);

    m_framebufferObjects["fbo_2"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
}

/**
  Called to switch to an orthogonal OpenGL camera.
  Useful for rending a textured quad across the whole screen.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::applyOrthogonalCamera(float width, float height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.f, width, 0.f, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
  Called to switch to a perspective OpenGL camera.

  @param width: the viewport width
  @param height: the viewport height
**/
static Vector3 fromAngles(float theta, float phi) { return Vector3(cosf(theta) * cosf(phi), sinf(phi), sinf(theta) * cosf(phi)); }
void GLWidget::applyPerspectiveCamera(float width, float height)
{
    float ratio = ((float) width) / height;
    Vector3 dir(-fromAngles(m_camera.theta, m_camera.phi));
    Vector3 eye(m_camera.center - dir * m_camera.zoom);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy, ratio, 0.1f, 1000.f);
    gluLookAt(eye.x, eye.y, eye.z, eye.x + dir.x, eye.y + dir.y, eye.z + dir.z,
              m_camera.up.x, m_camera.up.y, m_camera.up.z);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
  Draws the scene to a buffer which is rendered to the screen when this function exits.
 **/
void GLWidget::paintGL()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the fps
    int time = m_clock.elapsed();
    m_fps = 1000.f / (time - m_prevTime);
    m_prevTime = time;
    int width = this->width();
    int height = this->height();

    // Render the scene to a framebuffer
    m_framebufferObjects["fbo_0"]->bind();
    applyPerspectiveCamera(width, height);
    renderScene();
    m_framebufferObjects["fbo_0"]->release();

    // Copy the rendered scene into framebuffer 1(m_textures[0]
    m_framebufferObjects["fbo_0"]->blitFramebuffer(m_framebufferObjects["fbo_1"],
                                                   QRect(0, 0, width, height), m_framebufferObjects["fbo_0"],
                                                   QRect(0, 0, width, height), GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // TODO: Step 0 - draw the scene to the screen as a textured quad

    applyOrthogonalCamera(width,height);

    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_0"]->texture());

    renderTexturedQuad(width, height);

    glBindTexture(GL_TEXTURE_2D, 0);

    // TODO: Step 1 - use the brightpass shader to render bright areas
    // only to fbo_2


    m_framebufferObjects["fbo_2"]->bind();
    m_shaderPrograms["brightpass"]->bind();

    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_1"]->texture());

    renderTexturedQuad(width,height);

    m_shaderPrograms["brightpass"]->release();

    glBindTexture(GL_TEXTURE_2D, 0);

    m_framebufferObjects["fbo_2"]->release();


    // TODO: Uncomment this section in step 2 of the lab
//    float scales[] = {4.f,8.f};
//    for (int i = 0; i < 2; ++i)
//    {
//        // Render the blurred brightpass filter result to fbo 1
//       renderBlur(width / scales[i], height / scales[i]);

//       // Bind the image from fbo to a texture
//        glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_1"]->texture());
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

//        // Enable alpha blending and render the texture to the screen
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_ONE, GL_ONE);
//        renderTexturedQuad(width * scales[i], height * scales[i]);
//        glDisable(GL_BLEND);
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }

    paintText();
}

/**
  Renders the scene.  May be called multiple times by paintGL() if necessary.
**/
void GLWidget::renderScene()
{
    // Enable depth testing
   glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enable cube maps and draw the skybox
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
    glCallList(m_skybox);

    // Enable culling (back) faces for rendering the dragon
    glEnable(GL_CULL_FACE);

    // Render the dragon with the refraction shader bound

    // glPushMatrix();
    // glTranslatef(-1.25f, 0.f, 0.f);
    //glCallList(m_dragon.idx);


    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

    glDisable(GL_TEXTURE_CUBE_MAP);
  glEnable(GL_DEPTH_TEST);
 glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    QList<Planet*> planets = m_pms.getPlanets();
    int size=planets.size();
    triangle_t* tris;
    triangle_t tri;
    int numTriangles;
    glMatrixMode(GL_MODELVIEW);

    for (int j=0;j<size;j++) {
        Planet *planet = planets.at(j);

        // setting up shader

        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
        glLoadIdentity();
        Matrix4x4 tot=planet->getTotalTrans();
        tot=tot.getTranspose();
        glMultMatrixd(tot.data);
        double u,v,theta,psi,x,y,z;
        int temp;
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_textures[planet->getTexture()]);
       // glBindTexture(GL_TEXTURE_2D, m_textures[0]);
        m_shaderPrograms["planetShader"]->bind();
        m_shaderPrograms["planetShader"]->setUniformValue("planet_texture", 0);
        m_shaderPrograms["planetShader"]->setUniformValue("colorR", QVector3D(planet->getR().x,planet->getR().y,planet->getR().z));
        m_shaderPrograms["planetShader"]->setUniformValue("colorG", QVector3D(planet->getG().x,planet->getG().y,planet->getG().z));
        m_shaderPrograms["planetShader"]->setUniformValue("colorB", QVector3D(planet->getB().x,planet->getB().y,planet->getB().z));

        GLUquadricObj *sphere=planet->get_sphere();

        gluQuadricNormals(sphere, GLU_SMOOTH);
        gluQuadricTexture(sphere, GL_TRUE);
        gluSphere(sphere,planet->get_radius(),100,100);
        glPopMatrix();
        m_shaderPrograms["planetShader"]->release();
        glBindTexture(GL_TEXTURE_2D,0);
 }
    // Render the dragon with the reflection shader bound
   // m_shaderPrograms["reflect"]->bind();
   // m_shaderPrograms["reflect"]->setUniformValue("CubeMap", GL_TEXTURE0);
  //  glPushMatrix();
 //   glTranslatef(1.25f,0.f,0.f);
    //glCallList(m_dragon.idx);
 //   glPopMatrix();

//    m_shaderPrograms["reflect"]->release();
    // Disable culling, depth testing and cube maps
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

}

/**
  Run a gaussian blur on the texture stored in fbo 2 and
  put the result in fbo 1.  The blur should have a radius of 2.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::renderBlur(int width, int height)
{
    int radius = 2;
    int dim = radius * 2 + 1;
    GLfloat kernel[dim * dim];
    GLfloat offsets[dim * dim * 2];
    createBlurKernel(radius, width, height, &kernel[0], &offsets[0]);


    m_framebufferObjects["fbo_1"]->bind();
    m_shaderPrograms["blur"]->bind();

    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_2"]->texture());

    m_shaderPrograms["blur"]->setUniformValueArray("offsets",offsets,dim*dim*2,2);
    m_shaderPrograms["blur"]->setUniformValueArray("kernel",kernel,dim*dim,1);
    m_shaderPrograms["blur"]->setUniformValue("arraySize",dim);

    renderTexturedQuad(width,height);

    m_shaderPrograms["blur"]->release();

    glBindTexture(GL_TEXTURE_2D, 0);

    m_framebufferObjects["fbo_1"]->release();


}

/**
  Called when the mouse is dragged.  Rotates the camera based on mouse movement.
**/
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    Vector2 pos(event->x(), event->y());
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        m_camera.mouseMove(pos - m_prevMousePos);
    }
    m_prevMousePos = pos;
}

/**
  Record a mouse press position.
 **/
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_prevMousePos.x = event->x();
    m_prevMousePos.y = event->y();
}

/**
  Called when the mouse wheel is turned.  Zooms the camera in and out.
**/
void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
    {
        m_camera.mouseWheel(event->delta());
    }
}

/**
  Called when the GLWidget is resized.
 **/
void GLWidget::resizeGL(int width, int height)
{
    // Resize the viewport
    glViewport(0, 0, width, height);

    // Reallocate the framebuffers with the new window dimensions
    foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
    {
        const QString &key = m_framebufferObjects.key(fbo);
        QGLFramebufferObjectFormat format = fbo->format();
        delete fbo;
        m_framebufferObjects[key] = new QGLFramebufferObject(width, height, format);
    }
}

/**
  Draws a textured quad. The texture must be bound and unbound
  before and after calling this method - this method assumes that the texture
  has been bound beforehand.

  @param w: the width of the quad to draw
  @param h: the height of the quad to draw
**/
void GLWidget::renderTexturedQuad(int width, int height) {
    // Clamp value to edge of texture when texture index is out of bounds
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Draw the  quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(width, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(width, height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0.0f, height);
    glEnd();
}

/**
  Creates a gaussian blur kernel with the specified radius.  The kernel values
  and offsets are stored.

  @param radius: The radius of the kernel to create.
  @param width: The width of the image.
  @param height: The height of the image.
  @param kernel: The array to write the kernel values to.
  @param offsets: The array to write the offset values to.
**/
void GLWidget::createBlurKernel(int radius, int width, int height,
                                                    GLfloat* kernel, GLfloat* offsets)
{
    int size = radius * 2 + 1;
    float sigma = radius / 3.0f;
    float twoSigmaSigma = 2.0f * sigma * sigma;
    float rootSigma = sqrt(twoSigmaSigma * M_PI);
    float total = 0.0f;
    float xOff = 1.0f / width, yOff = 1.0f / height;
    int offsetIndex = 0;
    for (int y = -radius, idx = 0; y <= radius; ++y)
    {
        for (int x = -radius; x <= radius; ++x,++idx)
        {
            float d = x * x + y * y;
            kernel[idx] = exp(-d / twoSigmaSigma) / rootSigma;
            total += kernel[idx];
            offsets[offsetIndex++] = x * xOff;
            offsets[offsetIndex++] = y * yOff;
        }
    }
    for (int i = 0; i < size * size; ++i)
    {
        kernel[i] /= total;
    }
}

/**
  Handles any key press from the keyboard
 **/
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_S:
        QImage qi = grabFrameBuffer(false);
        QString filter;
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG Image (*.png)"), &filter);
        qi.save(QFileInfo(fileName).absoluteDir().absolutePath() + "/" + QFileInfo(fileName).baseName() + ".png", "PNG", 100);
        break;
    }
}

/**
  Draws text for the FPS and screenshot prompt
 **/
void GLWidget::paintText()
{
    glColor3f(1.f, 1.f, 1.f);

    // Combine the previous and current framerate
    if (m_fps >= 0 && m_fps < 1000)
    {
       m_prevFps *= 0.95f;
       m_prevFps += m_fps * 0.05f;
    }

    // QGLWidget's renderText takes xy coordinates, a string, and a font
    renderText(10, 20, "FPS: " + QString::number((int) (m_prevFps)), m_font);
    //renderText(10, 35, "S: Save screenshot", m_font);
}
