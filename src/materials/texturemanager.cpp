#include "texturemanager.h"
#include "texture.h"

#include <QDebug>
#include <QString>

TextureManager::TextureManager(const QOpenGLShaderProgramPtr& shader)
    : m_shader(shader)
{}

TextureManager::~TextureManager() {}

shared_ptr<Texture> TextureManager::getTexture(const string& name)
{
    if(m_textures.find(name) != m_textures.end())
    {
        qDebug() << "Texture " << QString::fromStdString(name) << " found";
        return m_textures[name];
    }

    qDebug() << "Texture " << QString::fromStdString(name) << " not found";

    return shared_ptr<Texture>(nullptr);
}

shared_ptr<Texture> TextureManager::addTexture(const string& name, const string& filename)
{
    if(m_textures.find(name) != m_textures.end() && m_textures[name].get() != nullptr)
    {
        qDebug() << "Texture " << QString::fromStdString(name) << " already exists";
        return m_textures[name];
    }

    qDebug() << "Add " << QString::fromStdString(name) << " in textures list";
    m_textures[name] = shared_ptr<Texture>(new Texture(filename));

    return m_textures[name];
}
