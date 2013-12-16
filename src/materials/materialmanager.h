#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "abstractmaterialmanager.h"
#include "material.h"

#include <map>

using namespace std;

class MaterialManager : public AbstractMaterialManager
{

public:
    MaterialManager(GLuint programHandle);
    virtual ~MaterialManager();

    virtual shared_ptr<Material> getMaterial(const string& name);

    virtual shared_ptr<Material> addMaterial(const string& name,
                                             const QVector4D& ambientColor,
                                             const QVector4D& diffuseColor,
                                             const QVector4D& specularColor,
                                             const QVector4D& emissiveColor,
                                             float shininess,
                                             float shininessStrength,
                                               int twoSided,
                                               int blendMode,
                                              bool alphaBlending,
                                              bool hasTexture);

private:
    map<string, shared_ptr<Material>> m_materials;
    GLuint m_programHandle;
};

#endif // MATERIALMANAGER_H
