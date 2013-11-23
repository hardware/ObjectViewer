#include "scene.h"
#include "camera.h"
#include "mesh.h"

#include "../helpers/shaders.h"
#include "../materials/texture.h"

#include <QOpenGLFunctions_4_0_Core>

/**
 * @brief Constructeur paramétré
 *
 * @param parent Objet parent
 */
Scene::Scene(QObject *parent)
    : AbstractScene(parent),
      m_camera(new Camera(this)),
      m_mesh(new Mesh()),
      // m_normalMap(new Texture(QImage("assets/box/normal_map.jpg"))),
      m_panAngle(0.0f),
      m_tiltAngle(0.0f),
      m_v(),
      m_viewCenterFixed(false),
      m_metersToUnits(0.05f),
      m_time(0.0f),
      m_lightMode(PerFragmentBlinnPhong),
      m_lightModeSubroutines(LightModeCount),
      m_funcs(0)

{
    // Initialisation de la position et de l'orientation de la camera
    m_camera->setPosition(QVector3D(-8.0f, 6.0f, -7.0f));
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
    delete m_mesh;
    // delete m_normalMap;
}

void Scene::initialize()
{
    m_funcs = m_context->versionFunctions<QOpenGLFunctions_4_0_Core>();

    if( ! m_funcs )
    {
        qFatal("Requires OpenGL >= 4.0");
        exit(1);
    }

    m_funcs->initializeOpenGLFunctions();

    // Charge, compile et link le Vertex et Fragment Shader
    prepareShaders();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);
    // glCullFace(GL_FRONT);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    QOpenGLShaderProgramPtr shader = m_shader->shader();

    shader->bind();
    shader->setUniformValue("texColor", 0);
    shader->setUniformValue("texNormal", 1);

    m_mesh->init(shader);
    m_mesh->loadMesh("assets/tomcat/f14d.lwo");
    // m_mesh->loadMesh("assets/blackhawk/uh60.lwo");

    // m_normalMap->load();
    // m_normalMap->bind(GL_TEXTURE1);
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

    if(currentTime > 0) m_model.rotateY(currentTime/0.02f);

    QMatrix4x4 modelViewMatrix = m_camera->viewMatrix() * m_model.modelMatrix();
    QOpenGLShaderProgramPtr shader = m_shader->shader();
    shader->bind();

    // Set the fragment shader light mode subroutine
    m_funcs->glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_lightModeSubroutines[m_lightMode]);

    shader->setUniformValue("modelViewMatrix", modelViewMatrix);
    shader->setUniformValue("projectionMatrix", m_camera->projectionMatrix());

    m_mesh->render();

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

    m_shader->setVertexShader(":/resources/shaders/per-fragment-blinn-phong.vert");
    m_shader->setFragmentShader(":/resources/shaders/per-fragment-blinn-phong.frag");

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
        glPointSize(2.0f);
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

Object3D* Scene::getObject()
{
    return &m_model;
}

Camera* Scene::getCamera()
{
    return m_camera;
}
