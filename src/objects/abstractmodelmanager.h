#ifndef ABSTRACTMODELMANAGER_H
#define ABSTRACTMODELMANAGER_H

#include <string>
#include <memory>

using namespace std;

class AbstractModel;
// class Mesh;
// class ModelData;

class AbstractModelManager
{

public:
    AbstractModelManager();

    virtual AbstractModel* getModel(const string& name) = 0;
    virtual void loadModel(const string& name, const string& filename) = 0;

    // virtual unique_ptr<AbstractModel> createModel(const string& name) = 0;
    // virtual unique_ptr<AbstractModel> createModel(Mesh* mesh) = 0;
    // virtual unique_ptr<AbstractModel> createModel(vector<shared_ptr<ModelData>> modelData) = 0;
};

#endif // ABSTRACTMODELMANAGER_H
