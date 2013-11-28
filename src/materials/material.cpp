#include "material.h"

#include <QDebug>
#include <QString>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>

Material::Material(const string& name,
                   const QVector4D& ambientColor,
                   const QVector4D& diffuseColor,
                   const QVector4D& specularColor,
                   const QVector4D& emissiveColor,
                   float shininess,
                   float shininessStrength,
                   const QOpenGLShaderProgramPtr& shader)
    : m_name(name),
      m_ambientColor(ambientColor),
      m_diffuseColor(diffuseColor),
      m_specularColor(specularColor),
      m_emissiveColor(emissiveColor),
      m_shininess(shininess),
      m_shininessStrength(shininessStrength),
      m_shader(shader)
{
    init();
    sendToGPU();
}

Material::~Material() {}

void Material::init()
{
    QOpenGLContext* context = QOpenGLContext::currentContext();

    Q_ASSERT(context);

    m_funcs = context->versionFunctions<QOpenGLFunctions_4_3_Core>();
    m_funcs->initializeOpenGLFunctions();
}

void Material::sendToGPU()
{
    qDebug() << "Load material : " << QString::fromStdString(m_name) << " into memory";

    m_shader->setUniformValue("material.Ka", m_ambientColor);
    m_shader->setUniformValue("material.Kd", m_diffuseColor);
    m_shader->setUniformValue("material.Ks", m_specularColor);
    m_shader->setUniformValue("material.Ke", m_emissiveColor);

    m_shader->setUniformValue("material.shininess", m_shininess);
    m_shader->setUniformValue("material.shininessStrength", m_shininessStrength);

    // TODO : Il faut utiliser un Uniform Buffer Object !!

    /*
    BLOCK IN FRAGMENT SHADER
    uniform struct MaterialInfo
    {
        vec4 Ka;
        vec4 Kd;
        vec4 Ks;
        vec4 Ke;

        float shininess;
        float shininessStrength;
    } material;
    */
}
