#include "modelmanager.h"
#include "scene.h"
#include "model.h"
#include "abstractmodel.h"

#include <QDebug>
#include <QString>

ModelManager::ModelManager(Scene *scene)
    : m_scene(scene)
{}

ModelManager::~ModelManager() {}

AbstractModel* ModelManager::getModel(const string& name)
{
    if(m_models.find(name) != m_models.end())
    {
        qDebug() << "Model " << QString::fromStdString(name) << " found";
        return m_models[name].get();
    }

    qDebug() << "Model " << QString::fromStdString(name) << " not found";
    return nullptr;
}

void ModelManager::loadModel(const string& name, const string& filename)
{
    vector<shared_ptr<ModelData>> modelData = m_modelLoader.loadModel(name, filename);
    m_models[name] = unique_ptr<Model>(new Model(m_scene, modelData));
}
