QT += core gui opengl widgets

CONFIG += console debug

win32: LIBS += \
    -L$$PWD/libs/ -lassimp \
    -L$$PWD/libs/ -lCORE_RL_Magick++_

unix:!macx: LIBS += \
    -L/usr/lib/ -lassimp \
    -L/usr/lib/ -lMagick++-6.Q16HDRI

INCLUDEPATH += \
    /usr/lib \
    /usr/include

DEPENDPATH += \
    /usr/lib \
    /usr/include

RCC_DIR     = $$_PRO_FILE_PWD_/build/tmp/rcc
UI_DIR      = $$_PRO_FILE_PWD_/build/tmp/ui
MOC_DIR     = $$_PRO_FILE_PWD_/build/tmp/moc
OBJECTS_DIR = $$_PRO_FILE_PWD_/build/tmp/obj

CONFIG(debug, debug|release) {
    DESTDIR = $$_PRO_FILE_PWD_/build/debug
}
CONFIG(release, debug|release) {
    DESTDIR = $$_PRO_FILE_PWD_/build/release
}

# GCC FLAGS
QMAKE_CXXFLAGS += -std=c++0x

QMAKE_CXXFLAGS += -isystem "/usr/include/qt/QtCore" \
                  -isystem "/usr/include/qt/QtGui" \
                  -isystem "/usr/include/qt/QtOpenGL" \
                  -isystem "/usr/include/qt/QtWidgets" \
                  -isystem "/usr/include/qt/QtOpenGLExtensions" \
                  -isystem "$$PWD/includes" \
                  -isystem "$$PWD/build/tmp/moc"

QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra -Wfloat-equal -Wundef -Wpointer-arith \
                          -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 \
                          -Wwrite-strings -Wcast-qual -Wswitch-enum -Wconversion \
                          -Wnon-virtual-dtor -Wmissing-noreturn \
                          -Wredundant-decls -Winline -Wdouble-promotion -Winit-self \
                          -Wold-style-cast -Woverloaded-virtual -pedantic

HEADERS += \
    src/helpers/shaders.h \
    src/objects/scene.h \
    src/objects/object3d.h \
    src/objects/camera_p.h \
    src/objects/camera.h \
    src/objects/abstractscene.h \
    src/ui/window.h \
    src/ui/mainwindow.h \
    src/materials/texture.h \
    src/objects/mesh.h \
    src/materials/material.h \
    src/objects/abstractmeshmanager.h \
    src/objects/meshmanager.h \
    src/materials/abstractmaterialmanager.h \
    src/materials/materialmanager.h \
    src/materials/abstracttexturemanager.h \
    src/materials/texturemanager.h \
    src/objects/abstractmodel.h \
    src/objects/abstractmodelmanager.h \
    src/objects/modelmanager.h \
    src/objects/model.h \
    src/objects/modelloader.h \
    src/helpers/opengluniformbuffer.h \
    src/objects/light.h \
    src/helpers/MathUtils.h

SOURCES += \
    src/helpers/shaders.cpp \
    src/objects/scene.cpp \
    src/objects/object3d.cpp \
    src/objects/camera.cpp \
    src/objects/abstractscene.cpp \
    src/ui/window.cpp \
    src/ui/mainwindow.cpp \
    src/main.cpp \
    src/materials/texture.cpp \
    src/objects/mesh.cpp \
    src/materials/material.cpp \
    src/objects/abstractmeshmanager.cpp \
    src/objects/meshmanager.cpp \
    src/materials/abstractmaterialmanager.cpp \
    src/materials/materialmanager.cpp \
    src/materials/abstracttexturemanager.cpp \
    src/materials/texturemanager.cpp \
    src/objects/abstractmodel.cpp \
    src/objects/abstractmodelmanager.cpp \
    src/objects/modelmanager.cpp \
    src/objects/model.cpp \
    src/objects/modelloader.cpp \
    src/helpers/opengluniformbuffer.cpp \
    src/objects/light.cpp

RESOURCES += \
    shaders.qrc \
    images.qrc \
    textures.qrc

OTHER_FILES += \
    resources/shaders/basic.frag \
    resources/shaders/basic.vert \
    resources/shaders/lighting/rim-lighting.vert \
    resources/shaders/lighting/rim-lighting.frag \
    resources/shaders/lighting/point-light.vert \
    resources/shaders/lighting/per-vertex-phong.vert \
    resources/shaders/lighting/per-vertex-phong.frag \
    resources/shaders/lighting/per-fragment-phong.vert \
    resources/shaders/lighting/per-fragment-phong.frag \
    resources/shaders/lighting/per-fragment-blinn-phong.vert \
    resources/shaders/lighting/per-fragment-blinn-phong.frag \
    resources/shaders/lighting/normal-mapping.vert \
    resources/shaders/lighting/normal-mapping.frag
