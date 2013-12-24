#include "scene.h"
#include "camera.h"
#include "model.h"

#include "modelmanager.h"
#include "meshmanager.h"

#include "../materials/materialmanager.h"
#include "../materials/texturemanager.h"

#include "../helpers/shaders.h"

#include <QOpenGLFunctions_4_3_Core>

/**
 * @brief Constructeur paramétré
 *
 * @param parent Objet parent
 */
Scene::Scene(QObject* parent)
    : AbstractScene(parent),
      m_camera(new Camera(this)),
      m_light("light01"),
      m_v(),
      m_viewCenterFixed(false),
      m_panAngle(0.0f),
      m_tiltAngle(0.0f),
      m_time(0.0f),
      m_metersToUnits(0.05f),
      m_lightMode(PerFragmentPhong),
      m_lightModeSubroutines(LightModeCount),
      m_funcs(nullptr)
{
    // Initialisation de la position et de l'orientation de la camera
    m_camera->setPosition(QVector3D(-8.0f, 0.0f, -7.0f));
    m_camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
    m_camera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));

    for(int i = 1; i < LightModeCount; i++)
    {
        m_lightModeSubroutines[i] = i;
    }
}

Scene::~Scene()
{
    delete m_camera;
    m_camera = nullptr;
}

void Scene::initialize()
{
    m_funcs = m_context->versionFunctions<QOpenGLFunctions_4_3_Core>();

    if( ! m_funcs )
    {
        qFatal("Requires OpenGL >= 4.3");
        exit(1);
    }

    m_funcs->initializeOpenGLFunctions();

    // Charge, compile et link le Vertex et Fragment Shader
    prepareShaders();

    glClearColor(0.39f, 0.39f, 0.39f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);

    QOpenGLShaderProgramPtr shader = m_shader->shader();
    shader->bind();
    shader->setUniformValue("texColor", 0);
    // shader->setUniformValue("texNormal", 1);

    m_light.setType(Light::SpotLight);
    m_light.setUniqueColor(1.0, 1.0, 1.0);
    m_light.setAttenuation(1.0f, 0.14f, 0.07f);
    m_light.setIntensity(3.0f);

    m_modelManager = unique_ptr<AbstractModelManager>(new ModelManager(this));

    m_materialManager = make_shared<MaterialManager>(shader->programId());
    m_textureManager  = make_shared<TextureManager>();
    m_meshManager     = make_shared<MeshManager>();

    /*
    MODELS AVAILABLE :
        - MercedesBenzSLSAMG/sls_amg.lwo
        - blackhawk/uh60.lwo
        - US_APC/apc.lwo
        - tomcat/f14d.lwo
        - audi_s5/audis5.3ds
    */

    m_model = m_modelManager->loadModel("GTAV", "assets/blackhawk/uh60.lwo", shader);
}

void Scene::update(float t)
{
    const float dt = t - m_time;
    m_time = t;

    Camera::CameraTranslationOption option = m_viewCenterFixed
                                           ? Camera::DontTranslateViewCenter
                                           : Camera::TranslateViewCenter;

    m_camera->translate(m_v * dt * m_metersToUnits, option);

    if( ! qFuzzyIsNull(m_panAngle) )
    {
        m_camera->pan(m_panAngle, QVector3D(0.0f, 1.0f, 0.0f));
        m_panAngle = 0.0f;
    }

    if ( ! qFuzzyIsNull(m_tiltAngle) )
    {
        m_camera->tilt(m_tiltAngle);
        m_tiltAngle = 0.0f;
    }
}

void Scene::render(double currentTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the fragment shader light mode subroutine
    // m_funcs->glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_lightModeSubroutines[m_lightMode]);

    if(currentTime > 0)
    {
        m_object.rotateY(static_cast<float>(currentTime)/0.02f);
    }

    QMatrix4x4 modelViewMatrix = m_camera->viewMatrix() * m_object.modelMatrix();
    QMatrix3x3 normalMatrix = modelViewMatrix.normalMatrix();

    QOpenGLShaderProgramPtr shader = m_shader->shader();

    shader->bind();
    shader->setUniformValue("normalMatrix", normalMatrix);
    shader->setUniformValue("modelViewMatrix", modelViewMatrix);
    shader->setUniformValue("projectionMatrix", m_camera->projectionMatrix());

    m_model->render();

    m_light.setPosition(m_camera->position());
    m_light.setDirection(m_camera->viewCenter());
    m_light.render(shader, m_camera->viewMatrix());

    emit renderCycleDone();
}

void Scene::resize(int width, int height)
{
    glViewport(0, 0, width, height);

    if(m_camera->projectionType() == Camera::PerspectiveProjection)
    {
        float aspect = static_cast<float>(width) / static_cast<float>(height);

        m_camera->setPerspectiveProjection(m_camera->fieldOfView(),
                                           aspect,
                                           m_camera->nearPlane(),
                                           m_camera->farPlane());
    }
    else if(m_camera->projectionType() == Camera::OrthogonalProjection)
    {
        m_camera->setOrthographicProjection(m_camera->left(),
                                            m_camera->right(),
                                            m_camera->bottom(),
                                            m_camera->top(),
                                            m_camera->nearPlane(),
                                            m_camera->farPlane());
    }
}

void Scene::prepareShaders()
{
    m_shader = ShadersPtr(new Shaders);

    m_shader->setVertexShader(":/resources/shaders/basic.vert");
    m_shader->setFragmentShader(":/resources/shaders/basic.frag");

    m_shader->shader()->link();
}

void Scene::toggleFill(bool state)
{
    if(state)
    {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Scene::toggleWireframe(bool state)
{
    if(state)
    {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void Scene::togglePoints(bool state)
{
    if(state)
    {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
}

void Scene::togglePhong(bool state)
{
    if(state) m_lightMode = PerFragmentPhong;
}

void Scene::toggleBlinnPhong(bool state)
{
    if(state) m_lightMode = PerFragmentBlinnPhong;
}

void Scene::toggleRimLighting(bool state)
{
    if(state) m_lightMode = RimLighting;
}

void Scene::toggleAA(bool state)
{
    (state) ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
}

Object3D* Scene::getObject()
{
    return &m_object;
}

Camera* Scene::getCamera()
{
    return m_camera;
}

shared_ptr<AbstractMeshManager> Scene::meshManager()
{
    return m_meshManager;
}

shared_ptr<AbstractTextureManager> Scene::textureManager()
{
    return m_textureManager;
}

shared_ptr<AbstractMaterialManager> Scene::materialManager()
{
    return m_materialManager;
}
