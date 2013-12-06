#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "abstractmodel.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>
#include <memory>

#include <QOpenGLBuffer>

using namespace std;

class ModelLoader
{

public:
    ModelLoader();
    virtual ~ModelLoader();

    vector<shared_ptr<ModelData>> loadModel(const string& filename,
                                            const QOpenGLShaderProgramPtr& shader);

    QOpenGLVertexArrayObjectPtr getVAO();

private:
    MeshData loadMesh(unsigned int index,
                      unsigned int numVertices,
                      unsigned int numIndices,
                      const aiMesh* mesh);

    MaterialData loadMaterial(unsigned int index, const aiMaterial* material);
    TextureData  loadTexture(const string& filename, const aiMaterial* material);

    void prepareVertexBuffers();
    void prepareVertexContainers(const aiMesh* mesh);

    QOpenGLBuffer m_vertexPositionBuffer;
    QOpenGLBuffer m_vertexColorBuffer;
    QOpenGLBuffer m_vertexTexCoordBuffer;
    QOpenGLBuffer m_vertexNormalBuffer;
    QOpenGLBuffer m_vertexTangentBuffer;
    QOpenGLBuffer m_indexBuffer;

    vector<QVector3D> m_positions;
    vector<QVector4D> m_colors;
    vector<QVector2D> m_texCoords;
    vector<QVector3D> m_normals;
    vector<QVector3D> m_tangents;
    vector<unsigned int> m_indices;

    QOpenGLVertexArrayObjectPtr m_vao;
    QOpenGLShaderProgramPtr     m_shader;

};

#endif // MODELLOADER_H
