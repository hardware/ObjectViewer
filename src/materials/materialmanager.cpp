#include "materialmanager.h"
#include "material.h"

MaterialManager::MaterialManager(const QOpenGLShaderProgramPtr& shader)
    : m_shader(shader)
{}

MaterialManager::~MaterialManager() {}

shared_ptr<Material> MaterialManager::getMaterial(const string& name)
{
    if(m_materials.find(name) != m_materials.end())
    {
        return m_materials[name];
    }

    return shared_ptr<Material>(nullptr);
}

shared_ptr<Material> MaterialManager::addMaterial(const string& name,
                                                  const QVector4D& ambientColor,
                                                  const QVector4D& diffuseColor,
                                                  const QVector4D& specularColor,
                                                  const QVector4D& emissiveColor,
                                                  float shininess,
                                                  float shininessStrength)
{
    if(m_materials.find(name) != m_materials.end() && m_materials[name].get() != nullptr)
    {
        return m_materials[name];
    }

    m_materials[name] = make_shared<Material>(name,
                                              ambientColor,
                                              diffuseColor,
                                              specularColor,
                                              emissiveColor,
                                              shininess,
                                              shininessStrength);

    return m_materials[name];
}
