QT += core gui opengl widgets

CONFIG += console debug

win32: LIBS += \
    -L$$PWD/libs/ -lassimp \
    -L$$PWD/libs/ -lCORE_RL_Magick++_

unix:!macx: LIBS += \
    -L/usr/lib/ -lassimp \
    -L/usr/lib/ -lMagick++-6.Q16HDRI

INCLUDEPATH += \
    $$PWD/libs \
    $$PWD/includes \
    /usr/lib \
    /usr/include

DEPENDPATH += \
    $$PWD/libs \
    $$PWD/includes \
    /usr/lib \
    /usr/include

# GCC FLAGS
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder

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

OTHER_FILES += \
    resources/shaders/basic.vert \
    resources/shaders/basic.frag \
    resources/shaders/per-vertex-phong.vert \
    resources/shaders/per-vertex-phong.frag \
    resources/shaders/per-fragment-phong.vert \
    resources/shaders/per-fragment-phong.frag \
    resources/shaders/per-fragment-blinn-phong.frag \
    resources/shaders/per-fragment-blinn-phong.vert \
    resources/shaders/rim-lighting.vert \
    resources/shaders/rim-lighting.frag \
    resources/shaders/normal-mapping.vert \
    resources/shaders/normal-mapping.frag

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
    src/objects/modelloader.h

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
    src/objects/modelloader.cpp

RESOURCES += \
    resources.qrc
