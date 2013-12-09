#include "material.h"

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
      m_uniformsBuffer()
{
    init();
    sendToGPU();
}

Material::~Material() {}

void Material::init()
{
    m_uniformsBuffer.create();
    m_uniformsBuffer.setUsagePattern(OpenGLUniformBuffer::DynamicDraw);
    m_uniformsBuffer.bind();
    m_uniformsBuffer.allocate(sizeof(MaterialInfo));
}

void Material::sendToGPU()
{
    m_uniformsBuffer.bind(1);

    MaterialInfo* blockData = static_cast<MaterialInfo*>(
        m_uniformsBuffer.map(
            0,
            sizeof(MaterialInfo),
            OpenGLUniformBuffer::Write |
            OpenGLUniformBuffer::InvalidateBuffer
        )
    );

    blockData->Ka = m_ambientColor;
    blockData->Kd = m_diffuseColor;
    blockData->Ks = m_specularColor;
    blockData->Ke = m_emissiveColor;
    blockData->shininess = m_shininess;
    blockData->shininessStrength = m_shininessStrength;

    m_uniformsBuffer.unmap();
}
