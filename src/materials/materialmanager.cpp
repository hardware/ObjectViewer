#include "materialmanager.h"
#include "material.h"

#include <QDebug>
#include <QString>

MaterialManager::MaterialManager(const QOpenGLShaderProgramPtr &shader)
    : m_shader(shader)
{}

MaterialManager::~MaterialManager() {}

Material* MaterialManager::getMaterial(const string& name)
{
    if(m_materials.find(name) != m_materials.end())
    {
        qDebug() << "Material " << QString::fromStdString(name) << " found";
        return m_materials[name].get();
    }

    qDebug() << "Material " << QString::fromStdString(name) << " not found";

    return nullptr;
}

Material* MaterialManager::addMaterial(const string& name,
                                       const QVector4D& ambientColor,
                                       const QVector4D& diffuseColor,
                                       const QVector4D& specularColor,
                                       const QVector4D& emissiveColor,
                                       float shininess,
                                       float shininessStrength)
{
    if(m_materials.find(name) != m_materials.end() && m_materials[name].get() != nullptr)
    {
        qDebug() << "Material " << QString::fromStdString(name) << " already exists";
        return m_materials[name].get();
    }

    qDebug() << "Add " << QString::fromStdString(name) << " in materials list";
    m_materials[name] = unique_ptr<Material>(new Material(name,
                                                          ambientColor,
                                                          diffuseColor,
                                                          specularColor,
                                                          emissiveColor,
                                                          shininess,
                                                          shininessStrength,
                                                          m_shader));

    return m_materials[name].get();
}
