#ifndef SHADERS_H
#define SHADERS_H

#include <QOpenGLShaderProgram>
#include <QSharedPointer>

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

class Shaders
{

public:
    Shaders();
    ~Shaders();

    void setVertexShader(const QString& vertexShader);
    void setTessellationControlShader(const QString& tessellationControlShader);
    void setTessellationEvaluationShader(const QString& tessellationEvaluationShader);
    void setGeometryShader(const QString& geometryShader);
    void setFragmentShader(const QString& fragmentShader);
    void setComputeShader(const QString& computeShader);

    void setShader(const QOpenGLShaderProgramPtr& shader);
    QOpenGLShaderProgramPtr shader() const;

private:
     QOpenGLShaderProgramPtr m_shader;
};

typedef QSharedPointer<Shaders> ShadersPtr;

#endif // SHADERS_H
