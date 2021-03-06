QT += core gui opengl

TARGET = planets
TEMPLATE = app

INCLUDEPATH += lab \
    lib \
    math \
    support
DEPENDPATH += lab \
    lib \
    math \
    support

HEADERS += lab/glwidget.h \
    lib/targa.h \
    lib/glm.h \
    support/resourceloader.h \
    support/mainwindow.h \
    support/camera.h \
    shape/Shape.h \
    shape/sphere.h \
    shape/planet.h \
    jasonShapes/triangle.h \
    jasonShapes/torus.h \
    jasonShapes/point.h \
    jasonShapes/cylinder.h \
    jasonShapes/cube.h \
    jasonShapes/cone.h \
    shape/planetmaster.h \
    math/CS123Algebra.h \
    lib/CS123Common.h \
    math/stochastic.h \
    support/particleemitter.h \
    support/common.h \
    filter/gaussianblur.h \
    lab/raytracer.h

SOURCES += \
    lab/glwidget.cpp \
    lib/targa.cpp \
    lib/glm.cpp \
    support/resourceloader.cpp \
    support/mainwindow.cpp \
    support/main.cpp \
    support/camera.cpp \
    shape/planet.cpp \
    jasonShapes/triangle.cpp \
    jasonShapes/torus.cpp \
    jasonShapes/point.cpp \
    shape/planetmaster.cpp \
    shape/Shape.cpp \
    shape/sphere.cpp \
    math/CS123Matrix.cpp \
    math/stochastic.cpp \
    support/particleemitter.cpp \
    filter/gaussianblur.cpp \
    lab/raytracer.cpp



FORMS += mainwindow.ui \
    support/mainwindow.ui

OTHER_FILES += \
    shaders/refract.vert \
    shaders/refract.frag \
    shaders/reflect.vert \
    shaders/reflect.frag \
    shaders/brightpass.frag \
    shaders/blur.frag \
    shaders/terrain.vert \
    shaders/terrain.vars \
    shaders/terrain.frag \
    shaders/pulse.vert \
    shaders/pulse.vars \
    shaders/pulse.frag \
    shaders/planet.frag \
    shaders/metal.vert \
    shaders/metal.vars \
    shaders/metal.frag \
    shaders/glass.vert \
    shaders/glass.vars \
    shaders/glass.frag \
    shaders/cartoon.vert \
    shaders/cartoon.vars \
    shaders/cartoon.frag \
    shaders/planet.vert

RESOURCES +=
