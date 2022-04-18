#include "ModelManager.h"

#include "../Utils/Dictionary.h"
#include "Misc.h"

#include <limits.h>

namespace ModelManager
{
    struct Globals
    {
        Dictionary<StringWrapper32, ModelID, HashSW32> pathToIdMap;
        Dictionary<ModelID, Model, HashInt> idToModelMap;
    } globals;
}

void ModelManager::Preload(const char* const aPath)
{
    StringWrapper32 wrapper;
    strcpy(wrapper.str, aPath);
    
    ModelID newId = GetRandomValue(0, INT_MAX);
    globals.pathToIdMap.Insert(wrapper, newId);
    globals.idToModelMap.Insert(newId, LoadModel(aPath));
}

ModelID ModelManager::GetModelID(const char* const aPath)
{
    StringWrapper32 wrapper;
    strcpy(wrapper.str, aPath);
    ModelID* ptr = globals.pathToIdMap.Get(wrapper);

    if (ptr)
    {
        return *ptr;
    }
    else
    {
        ModelID newId = GetRandomValue(0, INT_MAX);
        globals.pathToIdMap.Insert(wrapper, newId);
        globals.idToModelMap.Insert(newId, LoadModel(aPath));

        return newId;
    }
}

Model* ModelManager::GetModel(ModelID anId)
{
    return globals.idToModelMap.Get(anId);
}

void ModelManager::Terminate()
{
    auto callback = [](auto&, Model& m, auto&)
    {
        UnloadModel(m);
    };
    globals.idToModelMap.ForEach(callback);

    globals.pathToIdMap.Clear();
    globals.idToModelMap.Clear();
}