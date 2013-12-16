#include "model.h"
#include "scene.h"
#include "mesh.h"
#include "abstractmeshmanager.h"

#include "../materials/texture.h"
#include "../materials/material.h"
#include "../materials/abstractmaterialmanager.h"
#include "../materials/abstracttexturemanager.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>

Model::Model(Scene* scene,
             const QOpenGLVertexArrayObjectPtr& vao)
    : m_scene(scene),
      m_vao(vao),
      m_funcs(nullptr)
{
    initialize();
}

Model::Model(Scene* scene,
             const QOpenGLVertexArrayObjectPtr& vao,
             vector<shared_ptr<ModelData>> modelData)
    : m_scene(scene),
      m_vao(vao),
      m_funcs(nullptr)
{
    initialize(modelData);
}

Model::~Model() {}

void Model::initialize(vector<shared_ptr<ModelData>> modelData)
{
    QOpenGLContext* context = QOpenGLContext::currentContext();

    Q_ASSERT(context);

    m_funcs = context->versionFunctions<QOpenGLFunctions_4_3_Core>();
    m_funcs->initializeOpenGLFunctions();

    m_meshManager     = m_scene->meshManager();
    m_textureManager  = m_scene->textureManager();
    m_materialManager = m_scene->materialManager();

    for(shared_ptr<ModelData>& data : modelData)
    {
        shared_ptr<Mesh> mesh = m_meshManager->getMesh(data->meshData.name);

        if(mesh == nullptr)
        {
            mesh = m_meshManager->addMesh(data->meshData.name,
                                          data->meshData.numIndices,
                                          data->meshData.baseVertex,
                                          data->meshData.baseIndex);
        }

        m_meshes.push_back(mesh);

        if(data->textureData.hasTexture)
        {
            shared_ptr<Texture> texture = m_textureManager->getTexture(data->textureData.filename);

            if(texture == nullptr)
            {
                texture = m_textureManager->addTexture(data->textureData.filename,
                                                       data->textureData.filename);
            }

            m_textures.push_back(texture);
        }
        else
        {
            m_textures.push_back(nullptr);
        }

        shared_ptr<Material> material = m_materialManager->getMaterial(data->materialData.name);

        if(material == nullptr)
        {
            material = m_materialManager->addMaterial(data->materialData.name,
                                                      data->materialData.ambientColor,
                                                      data->materialData.diffuseColor,
                                                      data->materialData.specularColor,
                                                      data->materialData.emissiveColor,
                                                      data->materialData.shininess,
                                                      data->materialData.shininessStrength,
                                                      data->materialData.twoSided,
                                                      data->textureData.hasTexture);
        }

        m_materials.push_back(material);
    }
}

void Model::destroy() {}

void Model::render()
{
    m_vao->bind();

    for(unsigned int i = 0; i < m_meshes.size(); i++)
    {
        if(m_textures[i] != nullptr) m_textures[i]->bind(GL_TEXTURE0);
        if(m_materials[i] != nullptr) m_materials[i]->bind();

        m_funcs->glDrawElementsBaseVertex(
            GL_TRIANGLES,
            m_meshes[i]->getNumIndices(),
            GL_UNSIGNED_INT,
            reinterpret_cast<void*>((sizeof(unsigned int)) * m_meshes[i]->getBaseIndex()),
            m_meshes[i]->getBaseVertex()
        );
    }

    m_vao->release();
}
