#include "glwidget.h"

#include <iostream>
#include <QFileDialog>
#include <QGLFramebufferObject>
#include <QGLShaderProgram>
#include <QApplication>
#include <QMouseEvent>
#include <QTime>
#include <QTimer>
#include <QWheelEvent>
#include "glm.h"
#include "common.h"
#include "CS123Algebra.h"
#include "filter/gaussianblur.h"
#include "raytracer.h"

#include <sstream>
using namespace std;


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
    solar=false;
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    setFocusPolicy(Qt::StrongFocus);

    keys = QList<int>();

    m_camera.center = Vector3(0.f, 0.f, 0.f);
    m_camera.up = Vector3(0.f, 1.f, 0.f);
    m_camera.zoom = 3.5f;
    m_camera.theta = M_PI * 1.5f, m_camera.phi = 0.2f;
    m_camera.fovy = 60.f;
    m_camera.fovx = this->width()/this->height()*m_camera.fovy;
    m_camera.far = 12000;

    pause = true;
    mouseEnabled = false;

    revolving = NULL;

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
        glDeleteTextures(1,&(m_textures[i]));
    }

    //delete m_emitters;
}

void GLWidget::revolveCamera() {

    Vector4 eye4 = Vector4(m_camera.eye.x,m_camera.eye.y,m_camera.eye.z,1);

    Vector4 res = (revolving->get_position() - eye4).getNormalized();
    m_camera.dir = Vector3(res.x,res.y,res.z);


}

/**
  Our loop.
**/
void GLWidget::tick()
{
    update();

    handleKeys();

    if (revolving != NULL)
        if (revolving )
        revolveCamera();

    if (!pause) {

        QList<Planet*> planets = m_pms.getPlanets();
        int size = planets.size();
        Planet *pI, *pJ;
        Vector4 positionI,positionJ,difference,accelerationJ,accelerationI, fromItoJ, fromJtoI, velocityI, velocityJ;
        Vector3 p3;
        double x,y,z;
        double max=4096;
        double G=1.0;
        if (solar){
            G=1.0;
        }
        double massI, massJ;
        double rI, rJ, distance,total, newR, volume,distance2;
        for (int i=0;i<size;i++) {
            pI = planets.at(i);
            positionI=pI->get_position();
            x=fabs(positionI.x);
            y=fabs(positionI.y);
            z=fabs(positionI.z);
            rI=pI->get_radius();
            massI=pI->get_mass();
            //printf("%f\t%f\t%f\n\n",v.x,v.y,v.z);
            fflush(stdout);
            velocityI=pI->get_velocity();
            p3=Vector3(positionI.x,positionI.y,positionI.z);
            if (x>=0.97*m_camera.eye.x+max||y>=0.97*m_camera.eye.y+max||z>=0.97*m_camera.eye.z+max){
                if (pI==revolving){
                    revolving=NULL;
                }
                m_pms.remove_planet(i);
                planets.removeAt(i);
                i--;
                size--;
            }
//            else if(m_camera.dir.dot(p3-m_camera.eye)<0){
//                if (pI==revolving){
//                    revolving=NULL;
//                }
//                m_pms.remove_planet(i);
//                planets.removeAt(i);
//                i--;
//                size--;
//            }
            else{
                //if (collide)
                for (int j=0;j<i;j++){
                    pJ=planets.at(j);
                    velocityJ=pJ->get_velocity();
                    massJ=pJ->get_mass();
                    positionJ=pJ->get_position();
                    rJ=pJ->get_radius();
                    total=rI+rJ;
                    difference=positionJ-positionI;
                    fromItoJ=difference;
                    fromJtoI=-difference;
                    fromJtoI.w=0;
                    fromItoJ.w=0;
                    fromJtoI.normalize();
                    fromItoJ.normalize();
                    distance=difference.getMagnitude();
                    distance2=difference.getMagnitude2();
                    if (distance<=total){
                        newR=pow(volume,1.0/3.0);
                        if (rI>=rJ){
                            if (rJ>3){
                                pI->set_radius(rI+1.0);
                                pJ->set_radius(rJ-2.0);
                            }
                            else{
                                if (pJ==revolving){
                                    revolving=NULL;
                                }
                                m_pms.remove_planet(j);
                                planets.removeAt(j);
                                i--;
                                j--;
                                size--;
                            }
                        }
                        else {
                            if (rI>3){
                                pJ->set_radius(rJ+1.0);
                                 pI->set_radius(rI-2.0);
                            }
                            else{
                                if (pI==revolving){
                                    revolving=NULL;
                                }
                                m_pms.remove_planet(i);
                                planets.removeAt(i);
                                i--;
                                size--;
                                j=i;
                            }
                        }
                    }
                    else{
                        //-r from M to m
                        //r = from m to M
                        accelerationJ=G*massI*fromJtoI/(120.0*distance2);
                        pJ->set_velocity(velocityJ+accelerationJ);
                        accelerationI=G*massJ*fromItoJ/(120.0*distance2);
                        pI->set_velocity(velocityI+accelerationI);
                    }

                }
            }
            pI->move();
        }
        int rando = rand();
        rando=rando%1000;
        int rm=10;
        if (solar){
            rm=-1;
        }
        if (rando<=rm){
            m_pms.addPlanet(m_camera.eye);
        }
    }
}

void GLWidget::handleKeys() {

    if (keys.contains(Qt::Key_W))
        m_camera.move(Vector3(0,1,0),10);
    if (keys.contains(Qt::Key_A))
        m_camera.move(Vector3(-1,0,0),10);
    if (keys.contains(Qt::Key_S))
        m_camera.move(Vector3(0,-1,0),10);
    if (keys.contains(Qt::Key_D))
        m_camera.move(Vector3(1,0,0),10);

    if (keys.contains(Qt::Key_Q))
        m_camera.move(Vector3(0,0,-1),10);
    if (keys.contains(Qt::Key_Z))
        m_camera.move(Vector3(0,0,1),10);
}

void GLWidget::create_solar_system(){
    QList<Planet*> planets = m_pms.getPlanets();
    int size = planets.size();
    for (int i=0;i<size;i++){
        m_pms.remove_planet(i);
        planets.removeAt(i);
        i--;
        size--;
    }
    m_pms.create_solar_system();
}

GLuint GLWidget::loadTexture(const QString &filename)
{
    // Make sure the image file tsts
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
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_textures=(GLuint *) malloc(TEXTURES*sizeof(GLuint));
    m_texture_colors= (Vector4 *) malloc(TEXTURES*sizeof(Vector4));
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
    m_glu_sphere=gluNewQuadric();
    int t;
    t=this->loadTexture("textures/cloud.jpg");
    if (t==-1){
        cout << "cloud texture failed." << endl;
    }
    else{
        m_textures[2]=t;
        m_texture_colors[2]=Vector4(141.0/255.0,146.0/255.0,137.0/255.0,1.0);
    }
    t=this->loadTexture("textures/other1.jpg");
    if (t==-1){
        cout << "other1 texture failed." << endl;
    }
    else{
        m_textures[1]=t;
        m_texture_colors[1]=Vector4(141.0/255.0,146.0/255.0,137.0/255.0,1.0);
    }
    t=this->loadTexture("textures/sun.jpg");
    if (t==-1){
        cout << "sun texture failed." << endl;
    }
    else{
        m_textures[3]=t;
        m_texture_colors[3]=Vector4(240.0/255.0,72.0/255.0,3.0/255.0,1.0);
    }
    t=this->loadTexture("textures/black.jpg");
    if (t==-1){
        cout << "black texture failed." << endl;
    }
    else{
        m_textures[4]=t;
        m_texture_colors[4]=Vector4(19.0/255.0,19.0/255.0,19.0/255.0,1.0);
    }
    t=this->loadTexture("textures/pluto.jpg");
    if (t==-1){
        cout << "pluto texture failed." << endl;
    }
    else{
        m_textures[5]=t;
        m_texture_colors[5]=Vector4(137.0/255.0,146.0/255.0,155.0/255.0,1.0);
    }
    t=this->loadTexture("textures/moon.jpg");
    if (t==-1){
        cout << "moon texture failed." << endl;
    }
    else{
        m_textures[6]=t;
        m_texture_colors[6]=Vector4(139.0/255.0,139.0/255.0,139.0/255.0,1.0);
    }
    t=this->loadTexture("textures/moon2.jpg");
    if (t==-1){
        cout << "moon2 texture failed." << endl;
    }
    else{
        m_textures[7]=t;
        m_texture_colors[7]=Vector4(139.0/255.0,139.0/255.0,139.0/255.0,1.0);
    }
    t=this->loadTexture("textures/mercury.jpg");
    if (t==-1){
        cout << "mercury texture failed." << endl;
    }
    else{
        m_textures[8]=t;
        m_texture_colors[8]=Vector4(159.0/255.0,112.0/255.0,52.0/255.0,1.0);
    }
    t=this->loadTexture("textures/venus.jpg");
    if (t==-1){
        cout << "venus texture failed." << endl;
    }
    else{
        m_textures[9]=t;
        m_texture_colors[9]=Vector4(177.0/255.0,89.0/255.0,16.0/255.0,1.0);
    }
    t=this->loadTexture("textures/earthpng.png");
    if (t==-1){
        cout << "earth texture failed." << endl;
    }
    else{
        m_textures[10]=t;
        m_texture_colors[10]=Vector4(72.0/255.0,85.0/255.0,115.0/255.0,1.0);
    }
    t=this->loadTexture("textures/neptune.jpg");
    if (t==-1){
        cout << "neptune texture failed." << endl;
    }
    else{
        m_textures[11]=t;
        m_texture_colors[11]=Vector4(30.0/255.0,81.0/255.0,146.0/255.0,1.0);
    }
    t=this->loadTexture("textures/uranus.jpg");
    if (t==-1){
        cout << "uranus texture failed." << endl;
    }
    else{
        m_textures[12]=t;
        m_texture_colors[12]=Vector4(106.0/255.0,175.0/255.0,202.0/255.0,1.0);
    }

    t=this->loadTexture("textures/saturnpng.png");
    if (t==-1){
        cout << "saturn texture failed." << endl;
    }
    else{
        m_textures[13]=t;
        m_texture_colors[13]=Vector4(206.0/255.0,171.0/255.0,134.0/255.0,1.0);
    }

    t=this->loadTexture("textures/jupiter.jpg");
    if (t==-1){
        cout << "jupiter texture failed." << endl;
    }
    else{
        m_textures[14]=t;
        m_texture_colors[14]=Vector4(154.0/255.0,151.0/255.0,144.0/255.0,1.0);
    }

    t=this->loadTexture("textures/sunpng.png");
    if (t==-1){
        cout << "sun texture failed." << endl;
    }
    else{
        m_textures[15]=t;
        m_texture_colors[15]=Vector4(220.0/255.0,96.0/255.0,14.0/255.0,1.0);
    }


    for (int i=0;i<8;i++) {
        int za = (i>3) ? -1 : 1;
        int zb = (i%2 == 0) ? -1 : 1;
        int zc = (i%4 == 2 || i%4 == 3) ? -1 : 1;

        m_emitters.append(new ParticleEmitter(float3(1.0f,1.0f,1.0f),float3(za*-1.0f,zb*-1.0f,zc*-1.0f),
                                              0.3f,4096*2,0.5f, 500));

        Matrix4x4 mat = getTransMat(Vector4(za*4096,
                                            zb*4096,
                                            zc*4096,
                                            1));
        m_emitterTrans.append(mat);
    }

    glClear(GL_ACCUM_BUFFER_BIT);


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

    m_pms.addPlanet(m_camera.eye);
    m_pms.addPlanet(m_camera.eye);m_pms.addPlanet(m_camera.eye);m_pms.addPlanet(m_camera.eye);

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
    m_shaderPrograms["planetShader"] = ResourceLoader::newShaderProgram(ctx, "shaders/planet.vert", "shaders/planet.frag");
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
//static Vector3 fromAngles(float theta, float phi) { return Vector3(cosf(theta) * cosf(phi), sinf(phi), sinf(theta) * cosf(phi)); }

void GLWidget::applyPerspectiveCamera(float width, float height)
{
    float ratio = ((float) width) / height;
    //Vector3 dir(-fromAngles(m_camera.theta, m_camera.phi));
    //Vector3 eye(m_camera.center - dir * m_camera.zoom);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy, ratio, 0.1f, 12000.f);

    gluLookAt(m_camera.eye.x, m_camera.eye.y, m_camera.eye.z,
              m_camera.eye.x + m_camera.dir.x, m_camera.eye.y + m_camera.dir.y, m_camera.eye.z + m_camera.dir.z,
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

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

    // Copy the rendered scene into framebuffer 1
    m_framebufferObjects["fbo_0"]->blitFramebuffer(m_framebufferObjects["fbo_1"],
                                                   QRect(0, 0, width, height), m_framebufferObjects["fbo_0"],
                                                   QRect(0, 0, width, height), GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // TODO: Step 0 - draw the scene to the screen as a textured quad

    applyOrthogonalCamera(width,height);

    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_1"]->texture());

    renderTexturedQuad(width, height);
    glBindTexture(GL_TEXTURE_2D, 0);

    swapBuffers();
    paintText();
}

/**
  Renders the scene.  May be called multiple times by paintGL() if necessary.
**/
void GLWidget::renderScene()
{
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClearColor(1,1,1,1);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enable cube maps and draw the skybox
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);

    glPushMatrix();

    glTranslatef(m_camera.eye.x*0.97,m_camera.eye.y*0.97,m_camera.eye.z*0.97);

    glCallList(m_skybox);

    glPopMatrix();

    // Enable culling (back) faces for rendering the dragon
    glEnable(GL_CULL_FACE);

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

    glDisable(GL_TEXTURE_CUBE_MAP);
    //glEnable(GL_DEPTH_TEST);
    //glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    QList<Planet*> planets = m_pms.getPlanets();
    int size=planets.size();
    glMatrixMode(GL_MODELVIEW);

    for (int j=0;j<size;j++) {
        Planet *planet = planets.at(j);


        // setting up shader

        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
        glLoadIdentity();
        Matrix4x4 tot=planet->get_total_trans();
        tot=tot.getTranspose();
        glMultMatrixd(tot.data);


        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,m_textures[planet->get_texture()]);
        m_shaderPrograms["planetShader"]->bind();
        m_shaderPrograms["planetShader"]->setUniformValue("planet_texture", 0);
        m_shaderPrograms["planetShader"]->setUniformValue("average_color",QVector4D(m_texture_colors[planet->get_texture()].x,m_texture_colors[planet->get_texture()].y,m_texture_colors[planet->get_texture()].z,m_texture_colors[planet->get_texture()].w));
        m_shaderPrograms["planetShader"]->setUniformValue("colorR", QVector3D(planet->get_r().x,planet->get_r().y,planet->get_r().z));
        m_shaderPrograms["planetShader"]->setUniformValue("colorG", QVector3D(planet->get_g().x,planet->get_g().y,planet->get_g().z));
        m_shaderPrograms["planetShader"]->setUniformValue("colorB", QVector3D(planet->get_b().x,planet->get_b().y,planet->get_b().z));

       // GLUquadricObj *sphere=planet->get_sphere();

        gluQuadricNormals(m_glu_sphere, GLU_SMOOTH);
        gluQuadricTexture(m_glu_sphere, GL_TRUE);
        gluSphere(m_glu_sphere,planet->get_radius(),100,100);
        glPopMatrix();
        m_shaderPrograms["planetShader"]->release();
        glBindTexture(GL_TEXTURE_2D,0);
 }

    glPushMatrix();
    glTranslatef(m_camera.eye.x*0.97,m_camera.eye.y*0.97,m_camera.eye.z*0.97);

    for (int i=0;i<m_emitters.size();i++) {
        m_emitters.at(i)->updateParticles();                           //Move the particles
        m_emitters.at(i)->drawParticles(m_emitterTrans.at(i));         //Draw the particles
    }
    glPopMatrix();

    // Disable culling, depth testing and cube maps
    glDisable(GL_CULL_FACE);
    //glDisable(GL_DEPTH_TEST);

}

/**
  Called when the mouse is dragged.  Rotates the camera based on mouse movement.
**/
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (mouseEnabled) {
        Vector2 pos(event->x(), event->y());
        //if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
        //{
            m_camera.mouseMove(pos - m_prevMousePos);
        //}

        m_prevMousePos = Vector2(width()/2,height()/2);
        QPoint glob = mapToGlobal(QPoint(width()/2,height()/2));
        QCursor::setPos(glob);
    }
}

/**
  Record a mouse press position.
 **/
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    setMouseTracking(true);
    mouseEnabled = true;
    setCursor(Qt::BlankCursor);

    m_prevMousePos.x = event->x();
    m_prevMousePos.y = event->y();

    Vector4 eye4 = Vector4(m_camera.eye.x,m_camera.eye.y,m_camera.eye.z,1);

    RayTracer r = RayTracer();
    Vector4 res = r.generateRayD(event->x(),event->y(),this->width(),this->height(),eye4,
                            m_camera.getViewingTransformation());
    RayTracer::Closest *c = r.performTrace(res,eye4,m_pms.getPlanets());

}

/**
  Called when the mouse wheel is turned.  Zooms the camera in and out.
**/
void GLWidget::wheelEvent(QWheelEvent *event)
{
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

Planet* GLWidget::findClosestPlanet() {

    QList<Planet*> planets = m_pms.getPlanets();

    double dist = -1;
    Planet* p;
    Vector4 eye4 = Vector4(m_camera.eye.x,m_camera.eye.y,m_camera.eye.z,1);

    for (int i=0;i<planets.size();i++) {

        double adist = fabs(planets.at(i)->get_position().getDistance(eye4));

        if (dist == -1 || adist < dist) {
            p = planets.at(i);
            dist = adist;
        }
    }
    return p;
}

/**
  Handles any key press from the keyboard
 **/
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        QApplication::quit();
    }
    if (event->key() == Qt::Key_Space) {
        pause = !pause;
        return;
    }
    if (event->key() == Qt::Key_R) {
        m_camera.eye = Vector3::zero();
        return;
    }

    if (event->key() == Qt::Key_C) {
        solar=false;
        QList<Planet*> planets = m_pms.getPlanets();
        int size = planets.size();
        for (int i=0;i<size;i++){
            m_pms.remove_planet(i);
            planets.removeAt(i);
            i--;
            size--;
        }
    }

    if (event->key() == Qt::Key_M) {
        if (revolving == NULL) {
            Planet* p = findClosestPlanet();
            revolving = p;
            pause = true;
            return;
        }
        else {
            revolving = NULL;
            pause = false;
        }
    }

    if (event->key() == Qt::Key_Backspace) {
        mouseEnabled = false;
        setCursor(Qt::PointingHandCursor);
    }

    if (event->key() == Qt::Key_L) {
        solar=true;
        this->create_solar_system();
        return;
    }
    if (event->key() == Qt::Key_Period) {
        QImage qi = grabFrameBuffer(false);

        GaussianBlur gf = GaussianBlur(10);
        qi = gf.filter(qi);

        QString filter;
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG Image (*.png)"), &filter);
        if (fileName.size() > 0)
            qi.save(QFileInfo(fileName).absoluteDir().absolutePath() + "/" + QFileInfo(fileName).baseName() + ".png", "PNG", 100);
        return;
    }

    if (event->isAutoRepeat())
        return;

    bool down = keys.contains(event->key());
    if (!down && event->type() == QEvent::KeyPress)
        keys.append(event->key());
    else if (down && event->type() == QEvent::KeyRelease)
        keys.removeAll(event->key());
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;

    bool down = keys.contains(event->key());
    if (!down && event->type() == QEvent::KeyPress)
        keys.append(event->key());
    else if (down && event->type() == QEvent::KeyRelease)
        keys.removeAll(event->key());
}
/**
  Draws text for the FPS and screenshot prompt
 **/
void GLWidget::paintText()
{
    glColor3f(1.f, 1.f, 1.f);

    // Combine the previous and current framerate
    if (m_fps >= 0.01 && m_fps < 1000)
    {
       m_prevFps *= 0.95f;
       m_prevFps += m_fps * 0.05f;
    }

    if (m_prevFps < 0.1)
        m_prevFps = 0.0f;

    std::string s;
    std::stringstream sendout;
    sendout << "FPS: " << (int) m_prevFps;
    s = sendout.str();



    // QGLWidget's renderText takes xy coordinates, a string, and a font
    renderText(10, 20, QString(s.data()), m_font);
    renderText(12, this->height() - 42, "Planets Inc. by jfedor and jfreilic");
    renderText(10, this->height() - 25, "Space: Pause, R: Reset Camera, Arrow Keys: Move, Escape: Quit", m_font);
    renderText(10, this->height() - 10, "M: Focus on Object, L: Launch Solar System (Beta)", m_font);
}

void GLWidget::adjustSize() {
    repaint();
}
