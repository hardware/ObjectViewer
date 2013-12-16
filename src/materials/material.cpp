#include "material.h"
#include <QDebug>

Material::Material(const string& name,
                   const QVector4D& ambientColor,
                   const QVector4D& diffuseColor,
                   const QVector4D& specularColor,
                   const QVector4D& emissiveColor,
                   float shininess,
                   float shininessStrength,
                     int twoSided,
                     int blendMode,
                    bool alphaBlending,
                    bool hasTexture,
                   GLuint programHandle)
    : m_name(name),
      m_ambientColor(ambientColor),
      m_diffuseColor(diffuseColor),
      m_specularColor(specularColor),
      m_emissiveColor(emissiveColor),
      m_shininess(shininess),
      m_shininessStrength(shininessStrength),
      m_twoSided(twoSided),
      m_blendMode(blendMode),
      m_alphaBlending(alphaBlending),
      m_hasTexture(hasTexture),
      m_uniformsBuffer(programHandle, "MaterialInfo", 7)
{
    init();
}

Material::~Material() {}

void Material::init()
{
    m_uniformsBuffer.create();
    m_uniformsBuffer.setUsagePattern(OpenGLUniformBuffer::DynamicDraw);
    m_uniformsBuffer.bind();

    const GLchar* uniformNames[7] =
    {
        "MaterialInfo.Ka",
        "MaterialInfo.Kd",
        "MaterialInfo.Ks",
        "MaterialInfo.Ke",
        "MaterialInfo.shininess",
        "MaterialInfo.shininessStrength",
        "MaterialInfo.hasTexture"
    };

    GLint offsets[7];

    m_uniformsBuffer.getBlockMembersData(uniformNames, OpenGLUniformBuffer::Offset, offsets);

    GLint blockSize = m_uniformsBuffer.getSize();
    vector<GLubyte> buffer(blockSize);
    fillBuffer(buffer, offsets);

    m_uniformsBuffer.allocate(blockSize, buffer.data());
    m_uniformsBuffer.assignBindingPoint(1);
}

void Material::bind()
{
    m_uniformsBuffer.bind(1);

    // Specifies whether meshes using this material
    // must be rendered with or without backface culling
    (m_twoSided != 1) ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);

    if(m_alphaBlending && m_blendMode != -1)
    {
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);

        switch(m_blendMode)
        {
        case BlendMode::Additive:
            glBlendFunc(GL_ONE, GL_ONE);
            break;

        case BlendMode::Default:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        }
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }

}

void Material::fillBuffer(vector<GLubyte>& buffer, GLint* offsets)
{
    (reinterpret_cast<float*>(buffer.data() + offsets[0]))[0] = m_ambientColor.x();
    (reinterpret_cast<float*>(buffer.data() + offsets[0]))[1] = m_ambientColor.y();
    (reinterpret_cast<float*>(buffer.data() + offsets[0]))[2] = m_ambientColor.z();
    (reinterpret_cast<float*>(buffer.data() + offsets[0]))[3] = m_ambientColor.w();

    (reinterpret_cast<float*>(buffer.data() + offsets[1]))[0] = m_diffuseColor.x();
    (reinterpret_cast<float*>(buffer.data() + offsets[1]))[1] = m_diffuseColor.y();
    (reinterpret_cast<float*>(buffer.data() + offsets[1]))[2] = m_diffuseColor.z();
    (reinterpret_cast<float*>(buffer.data() + offsets[1]))[3] = m_diffuseColor.w();

    (reinterpret_cast<float*>(buffer.data() + offsets[2]))[0] = m_specularColor.x();
    (reinterpret_cast<float*>(buffer.data() + offsets[2]))[1] = m_specularColor.y();
    (reinterpret_cast<float*>(buffer.data() + offsets[2]))[2] = m_specularColor.z();
    (reinterpret_cast<float*>(buffer.data() + offsets[2]))[3] = m_specularColor.w();

    (reinterpret_cast<float*>(buffer.data() + offsets[3]))[0] = m_emissiveColor.x();
    (reinterpret_cast<float*>(buffer.data() + offsets[3]))[1] = m_emissiveColor.y();
    (reinterpret_cast<float*>(buffer.data() + offsets[3]))[2] = m_emissiveColor.z();
    (reinterpret_cast<float*>(buffer.data() + offsets[3]))[3] = m_emissiveColor.w();

    *(reinterpret_cast<float*>(buffer.data() + offsets[4])) = m_shininess;
    *(reinterpret_cast<float*>(buffer.data() + offsets[5])) = m_shininessStrength;

    *(reinterpret_cast<bool*>(buffer.data() + offsets[6])) = m_hasTexture;
}
