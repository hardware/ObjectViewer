#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "abstracttexturemanager.h"

#include <map>

#include <QOpenGLShaderProgram>
#include <QSharedPointer>

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

using namespace std;

class Texture;

class TextureManager : public AbstractTextureManager
{

public:
    TextureManager(const QOpenGLShaderProgramPtr& shader);
    virtual ~TextureManager();

    virtual shared_ptr<Texture> getTexture(const string& name);
    virtual shared_ptr<Texture> addTexture(const string& name, const string& filename);

private:
    map<string, shared_ptr<Texture>> m_textures;
    QOpenGLShaderProgramPtr m_shader;
};

#endif // TEXTUREMANAGER_H
