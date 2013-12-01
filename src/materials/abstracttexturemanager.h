#ifndef ABSTRACTTEXTUREMANAGER_H
#define ABSTRACTTEXTUREMANAGER_H

#include <string>
#include <memory>

using namespace std;

class Texture;

class AbstractTextureManager
{

public:
    AbstractTextureManager();
    virtual ~AbstractTextureManager() = 0;

    virtual shared_ptr<Texture> getTexture(const string& name) = 0;
    virtual shared_ptr<Texture> addTexture(const string& name, const string& filename) = 0;

};


#endif // ABSTRACTTEXTUREMANAGER_H
