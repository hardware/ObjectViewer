#include "mesh.h"

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLVertexArrayObject>

Mesh::Mesh(const QString& name,
           const QVector<QVector3D>& positions,
           const QVector<QVector4D>& colors,
           const QVector<QVector2D>& texCoords,
           const QVector<QVector3D>& normals,
           const QVector<QVector3D>& tangents,
           const QOpenGLShaderProgramPtr& shader)
    : m_funcs(0),
      m_vao(new QOpenGLVertexArrayObject),
      m_vertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexColorBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexTexCoordBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexNormalBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexTangentBuffer(QOpenGLBuffer::VertexBuffer),
      m_name(name),
      m_positions(positions),
      m_colors(colors),
      m_texCoords(texCoords),
      m_normals(normals),
      m_tangents(tangents),
      m_shader(shader)
{
    load();
}

Mesh::~Mesh()
{
    m_vertexPositionBuffer.destroy();
    m_vertexColorBuffer.destroy();
    m_vertexTexCoordBuffer.destroy();
    m_vertexNormalBuffer.destroy();
    m_vertexTangentBuffer.destroy();

    m_vao->destroy();

    delete m_vao;
}

void Mesh::init()
{
    QOpenGLContext* context = QOpenGLContext::currentContext();

    Q_ASSERT(context);

    m_funcs = context->versionFunctions<QOpenGLFunctions_4_3_Core>();
    m_funcs->initializeOpenGLFunctions();
}

void Mesh::load()
{
    qDebug() << "Loading mesh " << m_name << " into video memory";

    m_vao->create();
    m_vao->bind();

    m_vertexPositionBuffer.create();
    m_vertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexPositionBuffer.bind();
    m_vertexPositionBuffer.allocate(m_positions.constData(), m_positions.size() * sizeof(QVector3D));

    m_vertexColorBuffer.create();
    m_vertexColorBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexColorBuffer.bind();
    m_vertexColorBuffer.allocate(m_colors.constData(), m_colors.size() * sizeof(QVector4D));

    m_vertexTexCoordBuffer.create();
    m_vertexTexCoordBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexTexCoordBuffer.bind();
    m_vertexTexCoordBuffer.allocate(m_texCoords.constData(), m_texCoords.size() * sizeof(QVector2D));

    m_vertexNormalBuffer.create();
    m_vertexNormalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexNormalBuffer.bind();
    m_vertexNormalBuffer.allocate(m_normals.constData(), m_normals.size() * sizeof(QVector3D));

    m_vertexTangentBuffer.create();
    m_vertexTangentBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexTangentBuffer.bind();
    m_vertexTangentBuffer.allocate(m_tangents.constData(), m_tangents.size() * sizeof(QVector3D));

    m_shader->bind();

    m_vertexPositionBuffer.bind();
    m_shader->enableAttributeArray("position");
    m_shader->setAttributeBuffer("position", GL_FLOAT, 0, 3);

    m_vertexColorBuffer.bind();
    m_shader->enableAttributeArray("color");
    m_shader->setAttributeBuffer("color", GL_FLOAT, 0, 4);

    m_vertexTexCoordBuffer.bind();
    m_shader->enableAttributeArray("texCoord");
    m_shader->setAttributeBuffer("texCoord", GL_FLOAT, 0, 2);

    m_vertexNormalBuffer.bind();
    m_shader->enableAttributeArray("normal");
    m_shader->setAttributeBuffer("normal", GL_FLOAT, 0, 3);

    m_vertexTangentBuffer.bind();
    m_shader->enableAttributeArray("tangent");
    m_shader->setAttributeBuffer("tangent", GL_FLOAT, 0, 3);

    m_vao->release();

    qDebug() << "Successfully loaded mesh : " << m_name;
}

void Mesh::render()
{
    m_vao->bind();
    m_funcs->glDrawArrays(GL_TRIANGLES, 0, m_positions.size());
    m_vao->release();
}
