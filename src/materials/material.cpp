#include "material.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>

Material::Material(const string& name,
                   const QVector4D& ambientColor,
                   const QVector4D& diffuseColor,
                   const QVector4D& specularColor,
                   const QVector4D& emissiveColor,
                   float shininess,
                   float shininessStrength)
    : m_name(name),
      m_ambientColor(ambientColor),
      m_diffuseColor(diffuseColor),
      m_specularColor(specularColor),
      m_emissiveColor(emissiveColor),
      m_shininess(shininess),
      m_shininessStrength(shininessStrength),
      m_funcs(nullptr)
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

    m_funcs->glGenBuffers(1, &m_bufferId);
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
    m_funcs->glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialInfo), NULL, GL_DYNAMIC_DRAW);
}

void Material::sendToGPU()
{
    m_funcs->glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_bufferId);

    MaterialInfo* blockData = static_cast<MaterialInfo*>(
        m_funcs->glMapBufferRange(
            GL_UNIFORM_BUFFER,
            0,
            sizeof(MaterialInfo),
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
        )
    );

    blockData->Ka = m_ambientColor;
    blockData->Kd = m_diffuseColor;
    blockData->Ks = m_specularColor;
    blockData->Ke = m_emissiveColor;
    blockData->shininess = m_shininess;
    blockData->shininessStrength = m_shininessStrength;

    m_funcs->glUnmapBuffer(GL_UNIFORM_BUFFER);
}
