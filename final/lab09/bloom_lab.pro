QT += core gui opengl

TARGET = bloom_lab
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
    math/vector.h \
    support/resourceloader.h \
    support/mainwindow.h \
    support/camera.h \
    shape/Shape.h \
    shape/sphere.h \
    shape/planet.h \
    jasonShapes/triangle.h \
    jasonShapes/torus.h \
    jasonShapes/sphere.h \
    jasonShapes/point.h \
    jasonShapes/cylinder.h \
    jasonShapes/cube.h \
    jasonShapes/cone.h \
    shape/planetmaster.h

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
    jasonShapes/sphere.cpp \
    jasonShapes/point.cpp \
    shape/planetmaster.cpp



FORMS += mainwindow.ui \
    support/mainwindow.ui

OTHER_FILES += \
    shaders/refract.vert \
    shaders/refract.frag \
    shaders/reflect.vert \
    shaders/reflect.frag \
    shaders/brightpass.frag \
    shaders/blur.frag

RESOURCES +=
