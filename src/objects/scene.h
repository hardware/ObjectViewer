#ifndef SCENE_H
#define SCENE_H

#include "abstractscene.h"
#include "object3d.h"

#include <memory>

#include <qopengl.h>
#include <QSharedPointer>

using namespace std;

class Camera;
class Shaders;
class Model;

class AbstractMeshManager;
class AbstractMaterialManager;
class AbstractTextureManager;
class AbstractModelManager;

class QOpenGLFunctions_4_3_Core;

typedef QSharedPointer<Shaders> ShadersPtr;

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

    enum LightMode
    {
        PerFragmentBlinnPhong = 1,
        PerFragmentPhong,
        RimLighting,
        LightModeCount
    };

    void setLightMode(LightMode lightMode) { m_lightMode = lightMode; }
    LightMode lightMode() const { return m_lightMode; }

    Object3D* getObject();
    Camera*   getCamera();

    shared_ptr<AbstractMeshManager>     meshManager();
    shared_ptr<AbstractTextureManager>  textureManager();
    shared_ptr<AbstractMaterialManager> materialManager();

private:
    void prepareShaders();

public slots:
    void toggleFill(bool state);
    void toggleWireframe(bool state);
    void togglePoints(bool state);

    void togglePhong(bool state);
    void toggleBlinnPhong(bool state);
    void toggleRimLighting(bool state);

    void toggleAA(bool state);

signals:
    void renderCycleDone();

private:
    Camera* m_camera;

    shared_ptr<Model> m_model;

    unique_ptr<AbstractModelManager>    m_modelManager;
    shared_ptr<AbstractMeshManager>     m_meshManager;
    shared_ptr<AbstractTextureManager>  m_textureManager;
    shared_ptr<AbstractMaterialManager> m_materialManager;

    ShadersPtr m_shader;
    Object3D   m_object3D;
    QVector3D  m_v;

    LightMode       m_lightMode;
    QVector<GLuint> m_lightModeSubroutines;

    bool m_viewCenterFixed;

    float m_panAngle;
    float m_tiltAngle;
    float m_time;

    const float m_metersToUnits;

    QOpenGLFunctions_4_3_Core* m_funcs;
};

#endif // SCENE_H
