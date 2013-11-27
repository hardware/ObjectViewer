#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "abstractmaterialmanager.h"

#include <memory>
#include <map>

#include <QOpenGLShaderProgram>
#include <QSharedPointer>

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

using namespace std;

class Material;

class MaterialManager : public AbstractMaterialManager
{

public:
    MaterialManager(const QOpenGLShaderProgramPtr& shader);
    virtual ~MaterialManager();

    virtual Material* getMaterial(const string& name);

    virtual Material* addMaterial(const string& name,
                                  const QVector4D& ambientColor,
                                  const QVector4D& diffuseColor,
                                  const QVector4D& specularColor,
                                  const QVector4D& emissiveColor,
                                  float shininess,
                                  float shininessStrength);

private:
    map<string, unique_ptr<Material>> m_materials;
    QOpenGLShaderProgramPtr m_shader;
};

#endif // MATERIALMANAGER_H
