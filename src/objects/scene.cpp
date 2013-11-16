#include "scene.h"
#include "camera.h"
#include "mesh.h"

#include "../helpers/shaders.h"
#include "../materials/texture.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLDebugLogger>

/**
 * @brief Constructeur paramétré
 *
 * @param parent Objet parent
 */
Scene::Scene(QObject *parent)
    : AbstractScene(parent),
      m_camera(new Camera(this)),
      m_texture(new Texture(QImage(":/resources/images/grass.png"))),
      m_mesh(new Mesh()),
      m_vao(new QOpenGLVertexArrayObject(this)),
      m_logger(new QOpenGLDebugLogger(this)),
      m_vertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexTextureBuffer(QOpenGLBuffer::VertexBuffer),
      m_panAngle(0.0f),
      m_tiltAngle(0.0f),
      m_v(),
      m_viewCenterFixed(false),
      m_metersToUnits(0.05f),
      m_time(0.0f)

{
    // Initialisation de la position et de l'orientation de la camera
    m_camera->setPosition(QVector3D(0.0f, 0.6f, 2.0f));
    m_camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
    m_camera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
}

Scene::~Scene()
{
    delete m_camera;
    delete m_texture;
    delete m_mesh;

    m_vao->destroy();
}

void Scene::initialize()
{
    // Récupération des fonctions d'OpenGL 3.2
    m_funcs = m_context->versionFunctions<QOpenGLFunctions_3_2_Core>();

    if ( ! m_funcs )
    {
        qFatal("Requires OpenGL >= 3.2");
        exit(1);
    }

    m_funcs->initializeOpenGLFunctions();

    // Initialisation du système de logging
    connect(m_logger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

    if(m_logger->initialize())
        m_logger->enableMessages();

    // Création du Vertex Array Object
    m_vao->create();
    m_vao->bind();

    // Charge, compile et link le Vertex et Fragment Shader
    prepareShaders();

    // Initialisation du Vertex Buffer Object (VBO)
    prepareVertexBuffer();

    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    m_texture->load();

    QOpenGLShaderProgramPtr shader = m_shader->shader();
    shader->setUniformValue("gSampler", 0);

    m_mesh->Init(shader);
    m_mesh->LoadMesh("assets/tomcat/f14d.lwo");
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

    if(currentTime > 0)
    {
        m_spinningCube.rotateY(currentTime/0.02f);
    }

    QMatrix4x4 mvp = m_camera->viewProjectionMatrix() *
                     m_spinningCube.modelMatrix();

    m_shader->shader()->bind();
    m_shader->shader()->setUniformValue("mvp", mvp);

    m_mesh->Render();

    m_texture->bind(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

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

void Scene::prepareVertexBuffer()
{
    m_positionVertices.push_back(QVector3D(-10.0f, 0.0f, -10.0f));
    m_positionVertices.push_back(QVector3D( 10.0f, 0.0f, -10.0f));
    m_positionVertices.push_back(QVector3D( 10.0f, 0.0f,  10.0f));

    m_positionVertices.push_back(QVector3D(-10.0f, 0.0f, -10.0f));
    m_positionVertices.push_back(QVector3D(-10.0f, 0.0f,  10.0f));
    m_positionVertices.push_back(QVector3D( 10.0f, 0.0f,  10.0f));

    m_textureVertices.push_back(QVector2D(0.0f, 0.0f));
    m_textureVertices.push_back(QVector2D(7.0f, 0.0f));
    m_textureVertices.push_back(QVector2D(7.0f, 7.0f));

    m_textureVertices.push_back(QVector2D(0.0f, 0.0f));
    m_textureVertices.push_back(QVector2D(0.0f, 7.0f));
    m_textureVertices.push_back(QVector2D(7.0f, 7.0f));

    m_vertexPositionBuffer.create();
    m_vertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexPositionBuffer.bind();
    m_vertexPositionBuffer.allocate(m_positionVertices.constData(), m_positionVertices.size() * sizeof(QVector3D));
    m_vertexPositionBuffer.release();

    m_vertexTextureBuffer.create();
    m_vertexTextureBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexTextureBuffer.bind();
    m_vertexTextureBuffer.allocate(m_textureVertices.constData(), m_textureVertices.size() * sizeof(QVector2D));
    m_vertexTextureBuffer.release();

    m_shader->shader()->bind();

    m_vertexPositionBuffer.bind();
    m_shader->shader()->enableAttributeArray("vertexPosition");
    m_shader->shader()->setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

    m_vertexTextureBuffer.bind();
    m_shader->shader()->enableAttributeArray("TextureCoord");
    m_shader->shader()->setAttributeBuffer("TextureCoord", GL_FLOAT, 0, 2);
}

void Scene::onMessageLogged(QOpenGLDebugMessage message)
{
    qDebug() << message;
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

Object3D* Scene::getObject()
{
    return &m_spinningCube;
}

Camera* Scene::getCamera()
{
    return m_camera;
}
