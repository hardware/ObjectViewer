#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <QVector4D>

#include "../helpers/opengluniformbuffer.h"

using namespace std;

class Material
{

public:
    Material(const string& name,
             const QVector4D& ambientColor,
             const QVector4D& diffuseColor,
             const QVector4D& specularColor,
             const QVector4D& emissiveColor,
             float shininess,
             float shininessStrength);

    virtual ~Material();

    void setName(const string& name) { m_name = name; }
    string name() const { return m_name; }

    void sendToGPU();

private:
    void init();

    string m_name;

    QVector4D m_ambientColor;
    QVector4D m_diffuseColor;
    QVector4D m_specularColor;
    QVector4D m_emissiveColor;

    float m_shininess;
    float m_shininessStrength;

    int m_twoSided;
    int m_twoSidedTrue;
    int m_wireframe;

    // Uniform Block
    struct MaterialInfo
    {
        QVector4D Ka;
        QVector4D Kd;
        QVector4D Ks;
        QVector4D Ke;

        float shininess;
        float shininessStrength;
    };

    OpenGLUniformBuffer m_uniformsBuffer;

};

#endif // MATERIAL_H
