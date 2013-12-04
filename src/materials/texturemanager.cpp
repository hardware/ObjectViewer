#include "texturemanager.h"
#include "texture.h"

TextureManager::TextureManager(const QOpenGLShaderProgramPtr& shader)
    : m_shader(shader)
{}

TextureManager::~TextureManager() {}

shared_ptr<Texture> TextureManager::getTexture(const string& name)
{
    if(m_textures.find(name) != m_textures.end())
    {
        return m_textures[name];
    }

    return shared_ptr<Texture>(nullptr);
}

shared_ptr<Texture> TextureManager::addTexture(const string& name, const string& filename)
{
    if(m_textures.find(name) != m_textures.end() && m_textures[name].get() != nullptr)
    {
        return m_textures[name];
    }

    m_textures[name] = make_shared<Texture>(filename);

    qDebug() << "Loaded texture :" << QString::fromStdString(filename);

    return m_textures[name];
}
