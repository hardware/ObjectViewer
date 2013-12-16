#ifndef ABSTRACTMATERIALMANAGER_H
#define ABSTRACTMATERIALMANAGER_H

#include <string>
#include <memory>

#include <QVector4D>

using namespace std;

class Material;

class AbstractMaterialManager
{

public:
    AbstractMaterialManager();
    virtual ~AbstractMaterialManager() = 0;

    virtual shared_ptr<Material> getMaterial(const string& name) = 0;

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
                                              bool hasTexture) = 0;

};

#endif // ABSTRACTMATERIALMANAGER_H
