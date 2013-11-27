#ifndef ABSTRACTTEXTUREMANAGER_H
#define ABSTRACTTEXTUREMANAGER_H

#include <string>

using namespace std;

class Texture;

class AbstractTextureManager
{

public:
    AbstractTextureManager();

    virtual Texture* getTexture(const string& name) = 0;
    virtual Texture* addTexture(const string& name, const string& filename) = 0;

};


#endif // ABSTRACTTEXTUREMANAGER_H
