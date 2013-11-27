#ifndef ABSTRACTMATERIALMANAGER_H
#define ABSTRACTMATERIALMANAGER_H

#include <string>
#include <QVector4D>

using namespace std;

class Material;

class AbstractMaterialManager
{

public:
    AbstractMaterialManager();

    virtual Material* getMaterial(const string& name) = 0;

    virtual Material* addMaterial(const string& name,
                                  const QVector4D& ambientColor,
                                  const QVector4D& diffuseColor,
                                  const QVector4D& specularColor,
                                  const QVector4D& emissiveColor,
                                  float shininess,
                                  float shininessStrength) = 0;

};

#endif // ABSTRACTMATERIALMANAGER_H
