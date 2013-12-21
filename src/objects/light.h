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
        /// Point light sources give off light equally in all directions,
        /// so require only position not direction
        PointLight = 0,

        /// Directional lights simulate parallel light beams from a distant source,
        /// hence have direction but no position
        DirectionalLight = 1,

        /// Spotlights simulate a cone of light from a source so require position
        /// and direction, plus extra values for falloff
        SpotLight = 2
    };

    Light(const string& name);
    ~Light();

    void setType(LightType type);
    LightType getType() const;

    void setIntensity(float intensity);

    void setColors(const QVector3D& ambientColor,
                   const QVector3D& diffuseColor,
                   const QVector3D& specularColor);

    void setUniqueColor(const QVector3D& color);

    void setAmbientColor(float red, float green, float blue);
    void setDiffuseColor(float red, float green, float blue);
    void setSpecularColor(float red, float green, float blue);

    void setAmbientColor(const QVector3D& color);
    void setDiffuseColor(const QVector3D& color);
    void setSpecularColor(const QVector3D& color);

    const QVector3D& ambientColor() const;
    const QVector3D& diffuseColor() const;
    const QVector3D& specularColor() const;

    void setDirection(float x, float y, float z);
    void setDirection(const QVector3D& direction);

    const QVector3D& direction() const;

    void setPosition(float x, float y, float z);
    void setPosition(const QVector3D& position);

    const QVector3D& position() const;

    void setAttenuation(float range,
                        float constantFactor,
                        float linearFactor,
                        float quadraticFactor);

    void setRangeAttenuation(float range);
    void setConstantAttenuation(float constantFactor);
    void setLinearAttenuation(float linearFactor);
    void setQuadraticAttenuation(float quadraticFactor);

    float rangeAttenuation() const;
    float constantAttenuation() const;
    float linearAttenuation() const;
    float quadraticAttenuation() const;

    void setSpotInnerAngle(float innerAngle);
    void setSpotOuterAngle(float outerAngle);
    void setSpotFalloff(float falloff);

    void setSpotlightRange(float innerAngle, float outerAngle, float falloff = 1.0f);

    float spotInnerAngle() const;
    float spotOuterAngle() const;
    float spotFalloff() const;

    void render(const QOpenGLShaderProgramPtr& shader);

private:
    string    m_name;
    LightType m_type;

    QVector3D m_position;
    QVector3D m_direction;

    QColor m_ambientColor;
    QColor m_diffuseColor;
    QColor m_specularColor;

    float m_rangeAttenuation;
    float m_constantAttenuation;
    float m_linearAttenuation;
    float m_quadraticAttenuation;
    float m_spotInnerAngle;
    float m_spotOuterAngle;
    float m_spotFalloff;
    float m_intensity;

};

#endif // LIGHT_H
