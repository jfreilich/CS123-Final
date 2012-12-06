# 
# CS123 Final Project Starter Code
# Adapted from starter code graciously provided by CS195-U: 3D Game Engines
#

QT += core gui opengl

TARGET = finalProj
TEMPLATE = app

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
# INCLUDEPATH += folder1 folder2
# DEPENDPATH += folder1 folder2

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    support/resourceloader.cpp \
    support/targa.cpp \
    support/glm.cpp \
    support/camera.cpp \
    random.cpp \
    shape/sphere.cpp \
    shape/Shape.cpp


HEADERS += mainwindow.h \
    view.h \
    support/resourceloader.h \
    support/vector.h \
    support/targa.h \
    support/glm.h \
    support/camera.h \
    math/CS123Matrix.cpp \
    random.h \
    lib/CS123Common.h \
    shape/Shape.h \
    shape/sphere.h

FORMS += mainwindow.ui
