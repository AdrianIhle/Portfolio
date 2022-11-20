QT       += core gui widgets opengl
QT += core qml
TARGET = ORF_2017
TEMPLATE = app

INCLUDEPATH += .
INCLUDEPATH += ./include

mac {
    LIBS += -framework OpenAL
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win32\\libOpenAL32.dll.a
}

#If Qt 5.3 and 5.4 use this
equals(QT_MINOR_VERSION, 3): QMAKE_CXXFLAGS += -std=c++11
equals(QT_MINOR_VERSION, 4): QMAKE_CXXFLAGS += -std=c++11
#If Qt 5.5 use this:
greaterThan(QT_MINOR_VERSION, 4): CONFIG += c++11

SOURCES += main.cpp \
    camera.cpp \
    cube.cpp \
    gameobject.cpp\
    gameengine.cpp \
    axesgizmo.cpp \
    shaderprogram.cpp \
    editorwindow.cpp \
    material.cpp \
    circlesphere.cpp \
    transform.cpp \
    skybox.cpp \
    vertex.cpp \
    barycetrictriangulation.cpp \
    plane.cpp \
    interactiveball.cpp \
    oktaederball.cpp \
    readfile.cpp \
    mesh.cpp \
    meshbase.cpp \
    makemesh.cpp \
    boundingvolume.cpp \
    orfscene.cpp \
    component.cpp \
    audiolistener.cpp \
    audiosource.cpp \
    wavfilehandler.cpp \
    soundmanager.cpp \
    scriptcomponent.cpp \
    mesh_vertexbuffer.cpp \
    mesh_file.cpp

HEADERS += \
    transform.h \
    camera.h \
    cube.h \
    gameobject.h \
    gameengine.h \
    axesgizmo.h \
    shaderprogram.h \
    editorwindow.h \
    material.h \
    circlesphere.h \
    constants.h \
    skybox.h \
    vec3.h \
    vertex.h \
    vec2.h \
    barycetrictriangulation.h \
    plane.h \
    interactiveball.h \
    oktaederball.h \
    readfile.h \
    mesh.h \
    meshbase.h \
    makemesh.h \
    boundingvolume.h \
    orfscene.h \
    component.h \
    audiolistener.h \
    audiosource.h \
    wavfilehandler.h \
    soundmanager.h \
    scriptcomponent.h \
    mesh_vertexbuffer.h \
    mesh_file.h

FORMS += \
    EditorWindow.ui
