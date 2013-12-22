#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include <QColor>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QSharedPointer>

using namespace std;

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

class Light
{

public:
    enum LightType
    {
        PointLight,
        DirectionalLight,
        SpotLight
    };

    Light(const string& name);
    ~Light();

    void setType(LightType type);
    LightType type() const;

    void setIntensity(float intensity);

    void setUniqueColor(const QColor& color);
    void setUniqueColor(float r, float g, float b);

    void setColors(const QColor& ambientColor,
                   const QColor& diffuseColor,
                   const QColor& specularColor);

    void setAmbientColor(const QColor& color);
    void setAmbientColor(float r, float g, float b);

    void setDiffuseColor(const QColor& color);
    void setDiffuseColor(float r, float g, float b);

    void setSpecularColor(const QColor& color);
    void setSpecularColor(float r, float g, float b);

    const QColor& ambientColor() const;
    const QColor& diffuseColor() const;
    const QColor& specularColor() const;

    void setDirection(const QVector3D& direction);
    void setDirection(float x, float y, float z);

    const QVector3D& direction() const;

    void setPosition(const QVector3D& position);
    void setPosition(float x, float y, float z);

    const QVector3D& position() const;

    void setAttenuation(float constantFactor,
                        float linearFactor,
                        float quadraticFactor);

    void setConstantAttenuation(float constantFactor);
    void setLinearAttenuation(float linearFactor);
    void setQuadraticAttenuation(float quadraticFactor);

    float constantAttenuation() const;
    float linearAttenuation() const;
    float quadraticAttenuation() const;

    void setSpotFalloff(float falloff);
    void setSpotInnerAngle(float innerAngle);
    void setSpotOuterAngle(float outerAngle);

    float spotFallOff() const;
    float spotInnerAngle() const;
    float spotOuterAngle() const;

    void render(const QOpenGLShaderProgramPtr& shader, const QMatrix4x4& viewMatrix);

private:
    string    m_name;
    LightType m_type;

    QVector3D m_position;
    QVector3D m_direction;

    QColor m_ambientColor;
    QColor m_diffuseColor;
    QColor m_specularColor;

    float m_constantAttenuation;
    float m_linearAttenuation;
    float m_quadraticAttenuation;
    float m_spotFalloff;
    float m_spotInnerAngle;
    float m_spotOuterAngle;
    float m_intensity;

};

#endif // LIGHT_H
