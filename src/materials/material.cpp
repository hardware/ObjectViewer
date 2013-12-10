#include "material.h"

#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>

Material::Material(const string& name,
                   const QVector4D& ambientColor,
                   const QVector4D& diffuseColor,
                   const QVector4D& specularColor,
                   const QVector4D& emissiveColor,
                   float shininess,
                   float shininessStrength,
                   GLuint programHandle)
    : m_name(name),
      m_ambientColor(ambientColor),
      m_diffuseColor(diffuseColor),
      m_specularColor(specularColor),
      m_emissiveColor(emissiveColor),
      m_shininess(shininess),
      m_shininessStrength(shininessStrength),
      m_uniformsBuffer(),
      m_programHandle(programHandle),
      m_funcs(nullptr)
{
    init();
}

Material::~Material() {}

void Material::init()
{
    QOpenGLContext* context = QOpenGLContext::currentContext();

    Q_ASSERT(context);

    m_funcs = context->versionFunctions<QOpenGLFunctions_4_3_Core>();
    m_funcs->initializeOpenGLFunctions();

    m_funcs->glGenBuffers(1, &m_uniformHandle);
    m_funcs->glBindBuffer(GL_UNIFORM_BUFFER, m_uniformHandle);

    const GLchar* uniformNames[6] =
    {
        "MaterialInfo.Ka",
        "MaterialInfo.Kd",
        "MaterialInfo.Ks",
        "MaterialInfo.Ke",
        "MaterialInfo.shininess",
        "MaterialInfo.shininessStrength"
    };

    GLuint uniformIndices[6];
    m_funcs->glGetUniformIndices(m_programHandle, 6, uniformNames, uniformIndices);

    GLint uniformOffsets[6];
    m_funcs->glGetActiveUniformsiv(m_programHandle, 6, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);

    GLuint uniformBlockIndex = m_funcs->glGetUniformBlockIndex(m_programHandle, "MaterialInfo");

    GLint uniformBlockSize;
    m_funcs->glGetActiveUniformBlockiv(m_programHandle, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);

    const unsigned int uboSize = uniformBlockSize;
    vector<unsigned char> buffer(uboSize);

    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[0]))[0] = m_ambientColor.x();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[0]))[1] = m_ambientColor.y();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[0]))[2] = m_ambientColor.z();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[0]))[3] = m_ambientColor.w();

    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[1]))[0] = m_diffuseColor.x();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[1]))[1] = m_diffuseColor.y();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[1]))[2] = m_diffuseColor.z();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[1]))[3] = m_diffuseColor.w();

    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[2]))[0] = m_specularColor.x();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[2]))[1] = m_specularColor.y();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[2]))[2] = m_specularColor.z();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[2]))[3] = m_specularColor.w();

    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[3]))[0] = m_emissiveColor.x();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[3]))[1] = m_emissiveColor.y();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[3]))[2] = m_emissiveColor.z();
    (reinterpret_cast<float*>(&buffer[0] + uniformOffsets[3]))[3] = m_emissiveColor.w();

    *(reinterpret_cast<float*>(&buffer[0] + uniformOffsets[4])) = m_shininess;
    *(reinterpret_cast<float*>(&buffer[0] + uniformOffsets[5])) = m_shininessStrength;

    m_funcs->glBufferData(GL_UNIFORM_BUFFER, uboSize, &buffer[0], GL_DYNAMIC_DRAW);
    m_funcs->glUniformBlockBinding(m_programHandle, uniformBlockIndex, 1);

}

void Material::sendToGPU()
{
    m_funcs->glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_uniformHandle);
}
