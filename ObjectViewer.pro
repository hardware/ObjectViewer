QT += core gui opengl widgets

CONFIG +=  console debug

win32: LIBS += -L$$PWD/libs/ -lassimp

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

INCLUDEPATH += $$PWD/includes
DEPENDPATH += $$PWD/includes

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
    resources/shaders/basic.frag

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
    src/objects/mesh.h

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
    src/objects/mesh.cpp

RESOURCES += \
    resources.qrc
