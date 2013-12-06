#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "abstracttexturemanager.h"

#include <map>

using namespace std;

class Texture;

class TextureManager : public AbstractTextureManager
{

public:
    TextureManager();
    virtual ~TextureManager();

    virtual shared_ptr<Texture> getTexture(const string& name);
    virtual shared_ptr<Texture> addTexture(const string& name, const string& filename);

private:
    map<string, shared_ptr<Texture>> m_textures;

};

#endif // TEXTUREMANAGER_H
