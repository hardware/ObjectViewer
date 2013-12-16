#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <string>

#include <QVector4D>
#include <QSharedPointer>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

using namespace std;

struct MeshData
{
    string name;

    unsigned int numIndices;
    unsigned int baseVertex;
    unsigned int baseIndex;
};

struct TextureData
{
    string filename;
    bool hasTexture;
};

struct MaterialData
{
    string name;

    QVector4D ambientColor;
    QVector4D diffuseColor;
    QVector4D specularColor;
    QVector4D emissiveColor;

    float shininess;
    float shininessStrength;

    int twoSided;
    int blendMode;
    bool alphaBlending;
};

struct ModelData
{
    MeshData     meshData;
    TextureData  textureData;
    MaterialData materialData;
};

typedef QSharedPointer<QOpenGLShaderProgram> QOpenGLShaderProgramPtr;
typedef QSharedPointer<QOpenGLVertexArrayObject> QOpenGLVertexArrayObjectPtr;

class AbstractModel
{

public:
    AbstractModel();
    virtual ~AbstractModel() = 0;

    virtual void render() = 0;

};

#endif // ABSTRACTMODEL_H
