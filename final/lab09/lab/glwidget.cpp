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

using std::cout;
using std::endl;

/*
 box sizing localization

 orbit velocity around particular axis and on particular axes

 particle generation around planets / rings

 movement around box

 lytro effect w/ pausing/saving image
 */

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
    setCursor(Qt::BlankCursor);


    keys = QList<int>();

    m_camera.center = Vector3(0.f, 0.f, 0.f);
    m_camera.up = Vector3(0.f, 1.f, 0.f);
    m_camera.zoom = 3.5f;
    m_camera.theta = M_PI * 1.5f, m_camera.phi = 0.2f;
    m_camera.fovy = 60.f;

    pause = true;

    //m_emitter = ParticleEmitter(0);

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

    handleKeys();

    if (!pause) {

        QList<Planet*> planets = m_pms.getPlanets();
        int size = planets.size();
        Planet *pI, *pJ;
        Vector4 positionI,positionJ,difference,accelerationJ,accelerationI, fromItoJ, fromJtoI, velocityI, velocityJ;
        double x,y,z;
        double max=50000.0;
        double C=4.0*M_PI/3.0;
        double G=0.01;
        double massI, massJ;
        double rI, rJ, distance,total, newR, volume,distance2;
        for (int i=0;i<size;i++) {
            pI = planets.at(i);
            pI->move();
            positionI=pI->get_position();
            x=fabs(positionI.x);
            y=fabs(positionI.y);
            z=positionI.z;
            rI=pI->get_radius();
            massI=pI->get_mass();
            //printf("%f\t%f\t%f\n\n",v.x,v.y,v.z);
            fflush(stdout);
            velocityI=pI->get_velocity();
            if (x>=max||y>=max||z<=-max){
                m_pms.remove_planet(i);
                planets.removeAt(i);
                i--;
                size--;
            }
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
                        volume=(rI*rI*rI+rJ*rJ*rJ);
                        newR=pow(volume,1.0/3.0);
                        if (rI>=rJ){
    //                        p->set_radius(newR);
    //                        m_pms.remove_planet(j);
    //                        planets.removeAt(j);
    //                        i--;
    //                        j--;
    //                        size--;

                            if (rJ>5){
                                pI->set_radius(rI+1.0);
                                pJ->set_radius(rJ-4.0);
                            }
                            else{
                                m_pms.remove_planet(j);
                                planets.removeAt(j);
                                i--;
                                j--;
                                size--;
                            }

                        }
                        else {

                            if (rI>5){
                                pJ->set_radius(rJ+1.0);
                                 pI->set_radius(rI-4.0);
                            }
                            else{
                                m_pms.remove_planet(i);
                                planets.removeAt(i);
                                i--;
                                size--;
                                j=i;
                            }
    //                        p2->set_radius(newR);
    //                        m_pms.remove_planet(i);
    //                        planets.removeAt(i);
    //                        i--;
    //                        size--;
    //                        break;
                        }
                    }
                    else{
                        //-r from M to m
                        //r = from m to M
                        accelerationJ=G*massI*fromJtoI/distance2;
                        pJ->set_velocity(velocityJ+accelerationJ);
                        accelerationI=G*massJ*fromItoJ/distance2;
                        pI->set_velocity(velocityI+accelerationI);
                    }

                }
            }
        }
        int rando = rand();
        rando=rando%1000;
        if (rando<=50){
            m_pms.addPlanet(m_camera.eye);
        }
    }
}

void GLWidget::handleKeys() {

    if (keys.contains(Qt::Key_W))
        m_camera.move(Vector2(0,1),100);
    if (keys.contains(Qt::Key_A))
        m_camera.move(Vector2(-1,0),100);
    if (keys.contains(Qt::Key_S))
        m_camera.move(Vector2(0,-1),100);
    if (keys.contains(Qt::Key_D))
        m_camera.move(Vector2(1,0),100);
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
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    cout << "here" << endl ;
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
    GLuint t=this->loadTexture("textures/black.jpg");
    if (t==-1){
        cout << "black texture failed." << endl;
    }
    else{
        m_textures[0]=t;
        m_texture_colors[0]=Vector4(19.0/255.0,19.0/255.0,19.0/255.0,1.0);
    }
//    t=this->loadTexture("textures/earth3.png");
//    if (t==-1){
//        cout << "earth3 texture failed." << endl;
//    }
//    else{
//        m_textures[1]=t;
//        //m_texture_colors[1]=Vector4(112.0/255.0,112.0/255.0,112.0/255.0,1.0);
//        m_texture_colors[1]=Vector4(139.0/255.0,139.0/255.0,139.0/255.0,1.0);
//    }
    t=this->loadTexture("textures/cloud.jpg");
    if (t==-1){
        cout << "cloud texture failed." << endl;
    }
    else{
        m_textures[2]=t;
        m_texture_colors[2]=Vector4(141.0/255.0,146.0/255.0,137.0/255.0,1.0);
    }
    t=this->loadTexture("textures/earth.png");
    if (t==-1){
        cout << "earth texture failed." << endl;
    }
    else{
        m_textures[8]=t;
        m_texture_colors[8]=Vector4(72.0/255.0,85.0/255.0,115.0/255.0,1.0);


    }







    t=this->loadTexture("textures/green.jpg");
    if (t==-1){
        cout << "green texture failed." << endl;
    }
    else{
        m_textures[3]=t;
        m_texture_colors[3]=Vector4(69.0/255.0,115.0/255.0,113.0/255.0,1.0);
    }
    t=this->loadTexture("textures/jupiter.jpg");
    if (t==-1){
        cout << "jupiter texture failed." << endl;
    }
    else{
        m_textures[4]=t;
        m_texture_colors[4]=Vector4(201.0/255.0,150.0/255.0,109.0/255.0,1.0);
    }
    t=this->loadTexture("textures/sun.jpg");
    if (t==-1){
        cout << "sun texture failed." << endl;
    }
    else{
        m_textures[5]=t;
        m_texture_colors[5]=Vector4(240.0/255.0,72.0/255.0,3.0/255.0,1.0);
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

    t=this->loadTexture("textures/sea.png");
    if (t==-1){
        cout << "sea texture failed." << endl;
    }
    else{
        m_textures[9]=t;
        m_texture_colors[9]=Vector4(107.0/255.0,106.0/255.0,93.0/255.0,1.0);
    }
    t=this->loadTexture("textures/forest.png");
    if (t==-1){
        cout << "forest texture failed." << endl;
    }
    else{
        m_textures[10]=t;
        m_texture_colors[10]=Vector4(107.0/255.0,100.0/255.0,88.0/255.0,1.0);
    }
    t=this->loadTexture("textures/tundra.png");
    if (t==-1){
        cout << "tundra texture failed." << endl;
    }
    else{
        m_textures[11]=t;
        m_texture_colors[11]=Vector4(108.0/255.0,86.0/255.0,76.0/255.0,1.0);
    }
    t=this->loadTexture("textures/web.png");
    if (t==-1){
        cout << "web texture failed." << endl;
    }
    else{
        m_textures[12]=t;
        m_texture_colors[12]=Vector4(105.0/255.0,96.0/255.0,83.0/255.0,1.0);
    }
    t=this->loadTexture("textures/pluto.png");
    if (t==-1){
        cout << "pluto texture failed." << endl;
    }
    else{
        m_textures[13]=t;
        m_texture_colors[13]=Vector4(149.0/255.0,105.0/255.0,87.0/255.0,1.0);
    }
    t=this->loadTexture("textures/venus.png");
    if (t==-1){
        cout << "venus texture failed." << endl;
    }
    else{
        m_textures[14]=t;
        m_texture_colors[14]=Vector4(127.0/255.0,97.0/255.0,58.0/255.0,1.0);
    }
    t=this->loadTexture("textures/mercury.png");
    if (t==-1){
        cout << "mercury texture failed." << endl;
    }
    else{
        m_textures[15]=t;
        m_texture_colors[15]=Vector4(105.0/255.0,96.0/255.0,81.0/255.0,1.0);
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

    /*fileList.append(new QFile("stars/left.jpg"));
    fileList.append(new QFile("stars/front.jpg"));
    fileList.append(new QFile("stars/right.jpg"));
    fileList.append(new QFile("stars/back.jpg"));
    fileList.append(new QFile("stars/top.jpg"));
    fileList.append(new QFile("stars/bottom.jpg"));*/
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
static Vector3 fromAngles(float theta, float phi) { return Vector3(cosf(theta) * cosf(phi), sinf(phi), sinf(theta) * cosf(phi)); }
void GLWidget::applyPerspectiveCamera(float width, float height)
{
    float ratio = ((float) width) / height;
    //Vector3 dir(-fromAngles(m_camera.theta, m_camera.phi));
    //Vector3 eye(m_camera.center - dir * m_camera.zoom);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy, ratio, 0.1f, 500000.f);

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


//    m_framebufferObjects["fbo_2"]->bind();
//    m_shaderPrograms["brightpass"]->bind();

//    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_1"]->texture());

//    renderTexturedQuad(width,height);

//    m_shaderPrograms["brightpass"]->release();

//    glBindTexture(GL_TEXTURE_2D, 0);

//    m_framebufferObjects["fbo_2"]->release();


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

    glPushMatrix();

    glTranslatef(m_camera.eye.x*0.97,m_camera.eye.y*0.97,m_camera.eye.z*0.97);

    glCallList(m_skybox);

    glPopMatrix();

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

        /*particles*/
        glDisable(GL_DEPTH);
        glEnable(GL_BLEND);

        //planet->emitter->updateParticles();       //Move the particles
        //planet->emitter->drawParticles();         //Draw the particles

        glAccum(GL_MULT, .9f);

        glAccum(GL_ACCUM, 0.1f);
//
        glAccum(GL_RETURN, 1.0f);

        glFlush();
        /*particles*/




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

    //m_emitter.updateParticles();
    //m_emitter.drawParticles();

    // Render the dragon with the reflection shader bound
   // m_shaderPrograms["reflect"]->bind();
   // m_shaderPrograms["reflect"]->setUniformValue("CubeMap", GL_TEXTURE0);
  //  glPushMatrix();
 //   glTranslatef(1.25f,0.f,0.f);
    //glCallList(m_dragon.idx);
 //   glPopMatrix();

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
    //if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    //{
        m_camera.mouseMove(pos - m_prevMousePos);
    //}

    m_prevMousePos = Vector2(width()/2,height()/2);
    QPoint glob = mapToGlobal(QPoint(width()/2,height()/2));
    QCursor::setPos(glob);
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
        //m_camera.mouseWheel(event->delta());
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
    if (event->key() == Qt::Key_Escape) {
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        QApplication::quit();
    }
    if (event->key() == Qt::Key_Space) {
        pause = !pause;
    }
    if (event->key() == Qt::Key_R) {
        m_camera.eye = Vector3::zero();
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
    if (m_fps >= 0 && m_fps < 1000)
    {
       m_prevFps *= 0.95f;
       m_prevFps += m_fps * 0.05f;
    }

    // QGLWidget's renderText takes xy coordinates, a string, and a font
    renderText(10, 20, "FPS: " + QString::number((int) (m_prevFps)), m_font);
    renderText(10, 750, "Space: Pause, R: Reset Camera, Arrow Keys: Move, Escape: Quit", m_font);
}
