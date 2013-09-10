#ifndef SCENE_H
#define SCENE_H

#include "abstractscene.h"
#include "object3d.h"

#include "../helpers/shaders.h"

#include <QOpenGLBuffer>
#include <QOpenGLDebugMessage>

class Camera;
class QOpenGLVertexArrayObject;
class QOpenGLFunctions_3_2_Core;
class QOpenGLDebugLogger;

class Scene : public AbstractScene
{
    Q_OBJECT

public:
    Scene(QObject* parent = 0);
    virtual ~Scene();

    virtual void initialize();
    virtual void update(float t);
    virtual void render(double currentTime);
    virtual void resize(int width, int height);

    // Contrôle du mouvement de la camera
    inline void setSideSpeed(float vx)     { m_v.setX(vx); }
    inline void setVerticalSpeed(float vy) { m_v.setY(vy); }
    inline void setForwardSpeed(float vz)  { m_v.setZ(vz); }
    inline void setViewCenterFixed(bool b) { m_viewCenterFixed = b; }

    // Contrôle de l'orientation de la camera
    inline void pan(float angle)  { m_panAngle  = angle; }
    inline void tilt(float angle) { m_tiltAngle = angle; }

    Object3D* getObject();
    Camera*   getCamera();

private:
    void prepareShaders();
    void prepareVertexBuffer();

public slots:
    void onMessageLogged(QOpenGLDebugMessage message);

    void toggleFill(bool state);
    void toggleWireframe(bool state);
    void togglePoints(bool state);

signals:
    void renderCycleDone();

private:
    Camera                    * m_camera;
    QOpenGLVertexArrayObject  * m_vao;
    QOpenGLFunctions_3_2_Core * m_funcs;
    QOpenGLDebugLogger        * m_logger;
    QOpenGLBuffer               m_vertexPositionBuffer;
    QVector<QVector3D>          m_positionVertices;
    QMatrix4x4                  m_projection;

    ShadersPtr m_shader;
    Object3D   m_spinningCube;
    QVector3D  m_v;

    bool m_viewCenterFixed;

    float m_panAngle;
    float m_tiltAngle;
    float m_time;

    const float m_metersToUnits;
};

#endif // SCENE_H
