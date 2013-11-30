#include "model.h"
#include "scene.h"
#include "mesh.h"
#include "abstractmeshmanager.h"

#include "../materials/texture.h"
#include "../materials/material.h"
#include "../materials/abstractmaterialmanager.h"
#include "../materials/abstracttexturemanager.h"

#include <QDebug>

Model::Model(Scene* scene)
    : m_scene(scene)
{
    initialize();
}

Model::Model(Scene* scene, vector<shared_ptr<ModelData>> modelData)
    : m_scene(scene)
{
    initialize(modelData);
}

Model::Model(const Model& other)
{
    m_scene = other.m_scene;

    m_meshManager     = m_scene->meshManager();
    m_materialManager = m_scene->materialManager();
    m_textureManager  = m_scene->textureManager();

    m_meshes    = other.m_meshes;
    m_textures  = other.m_textures;
    m_materials = other.m_materials;
}

Model::~Model() {}

void Model::initialize(vector<shared_ptr<ModelData>> modelData)
{
    m_meshManager     = m_scene->meshManager();
    m_materialManager = m_scene->materialManager();
    m_textureManager  = m_scene->textureManager();

    for(shared_ptr<ModelData> data : modelData)
    {
        Mesh* mesh = m_meshManager->getMesh(data->meshData.name);

        if(mesh == nullptr)
        {
            mesh = m_meshManager->addMesh(data->meshData.name,
                                          data->meshData.positions,
                                          data->meshData.colors,
                                          data->meshData.texCoords,
                                          data->meshData.normals,
                                          data->meshData.tangents);
        }

        m_meshes.push_back(mesh);

        if( ! data->textureData.filename.empty() )
        {
            Texture* texture = m_textureManager->getTexture(data->textureData.filename);

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

        Material* material = m_materialManager->getMaterial(data->materialData.name);

        if(material == nullptr)
        {
            material = m_materialManager->addMaterial(data->materialData.name,
                                                      data->materialData.ambientColor,
                                                      data->materialData.diffuseColor,
                                                      data->materialData.specularColor,
                                                      data->materialData.emissiveColor,
                                                      data->materialData.shininess,
                                                      data->materialData.shininessStrength);
        }

        m_materials.push_back(material);
    }
}

void Model::destroy() {}

void Model::render(const QOpenGLShaderProgramPtr& shader)
{
    Q_UNUSED(shader);

    for(unsigned int i = 0; i < m_meshes.size(); i++)
    {
        if(m_textures[i] != nullptr)
        {
            m_textures[i]->bind(GL_TEXTURE0);
        }

        if(m_materials[i] != nullptr)
        {
            // TODO : Bind material
        }

        m_meshes[i]->render();
    }
}
