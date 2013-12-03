#ifndef ABSTRACTLIGHT_H
#define ABSTRACTLIGHT_H

#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QSharedPointer>

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

class AbstractLight
{

public:
    AbstractLight();
    virtual ~AbstractLight() = 0;

    void setColors(const QVector3D& ambientColor,
                   const QVector3D& diffuseColor,
                   const QVector3D& specularColor);

    void setIntensity(float intensity);
    void setUniqueColor(const QVector3D& color);

    void setAmbientColor(const QVector3D& color);
    void setDiffuseColor(const QVector3D& color);
    void setSpecularColor(const QVector3D& color);

    QVector3D ambientColor() const;
    QVector3D diffuseColor() const;
    QVector3D specularColor() const;

    virtual void render(const QOpenGLShaderProgramPtr& shader) = 0;

protected:
    float m_intensity;

    QVector3D m_ambientColor;
    QVector3D m_diffuseColor;
    QVector3D m_specularColor;

};

#endif // ABSTRACTLIGHT_H
