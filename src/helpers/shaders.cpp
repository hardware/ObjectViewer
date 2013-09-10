#include "shaders.h"

Shaders::Shaders()
    : m_shader(new QOpenGLShaderProgram)
{}

Shaders::~Shaders()
{
    m_shader->release();
}

void Shaders::setVertexShader(const QString &vertexShader)
{
    if( ! m_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader) )
        qCritical() << "Could not compile vertex shader. Log : " << m_shader->log();
}

void Shaders::setTessellationControlShader(const QString &tessellationControlShader)
{
    if( ! m_shader->addShaderFromSourceFile(QOpenGLShader::TessellationControl, tessellationControlShader) )
        qCritical() << "Could not compile tessellation control shader. Log : " << m_shader->log();
}

void Shaders::setTessellationEvaluationShader(const QString &tessellationEvaluationShader)
{
    if( ! m_shader->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, tessellationEvaluationShader) )
        qCritical() << "Could not compile tessellation evaluation shader. Log : " << m_shader->log();
}

void Shaders::setGeometryShader(const QString &geometryShader)
{
    if( ! m_shader->addShaderFromSourceFile(QOpenGLShader::Geometry, geometryShader) )
        qCritical() << "Could not compile geometry shader. Log : " << m_shader->log();
}

void Shaders::setFragmentShader(const QString &fragmentShader)
{
    if( ! m_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader) )
        qCritical() << "Could not compile fragment shader. Log : " << m_shader->log();
}

void Shaders::setComputeShader(const QString &computeShader)
{
    if( ! m_shader->addShaderFromSourceFile(QOpenGLShader::Compute, computeShader) )
        qCritical() << "Could not compile compute shader. Log : " << m_shader->log();
}

void Shaders::setShader(const QOpenGLShaderProgramPtr& shader)
{
    m_shader = shader;
}

QOpenGLShaderProgramPtr Shaders::shader() const
{
    return m_shader;
}
