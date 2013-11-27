#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <string>

#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QOpenGLShaderProgram>
#include <QSharedPointer>

using namespace std;

struct MeshData {
    string name;

    QVector<QVector3D> positions;
    QVector<QVector4D> colors;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;
    QVector<QVector3D> tangents;
};

struct MaterialData {
    string name;

    QVector4D ambientColor;
    QVector4D diffuseColor;
    QVector4D specularColor;
    QVector4D emissiveColor;

    float shininess;
    float shininessStrength;
};

struct TextureData {
    string filename;
};

struct ModelData {
    MeshData     meshData;
    MaterialData materialData;
    TextureData  textureData;
};

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;

class AbstractModel
{

public:
    AbstractModel();

    virtual void render(const QOpenGLShaderProgramPtr& shader) = 0;

};

#endif // ABSTRACTMODEL_H
