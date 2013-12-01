#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "abstractmodelmanager.h"
#include "modelloader.h"

class Scene;
class Model;
class AbstractModel;

class ModelManager : public AbstractModelManager
{

public:
    ModelManager(Scene* scene);
    virtual ~ModelManager();

    virtual AbstractModel* getModel(const string& name);
    virtual void loadModel(const string& name, const string& filename);
    virtual unique_ptr<AbstractModel> createModel(const string& name);

private:
    map<string, unique_ptr<Model>> m_models;
    Scene* m_scene;
    ModelLoader m_modelLoader;
};

#endif // MODELMANAGER_H
