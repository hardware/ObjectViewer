#ifndef ABSTRACTMODELMANAGER_H
#define ABSTRACTMODELMANAGER_H

#include <string>
#include <memory>

using namespace std;

class AbstractModel;
class Model;

class AbstractModelManager
{

public:
    AbstractModelManager();

    virtual AbstractModel* getModel(const string& name) = 0;
    virtual shared_ptr<Model> loadModel(const string& name, const string& filename) = 0;
//    virtual unique_ptr<AbstractModel> createModel(const string& name) = 0;
};

#endif // ABSTRACTMODELMANAGER_H
