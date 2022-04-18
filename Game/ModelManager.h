#if !defined(MODELMANAGER_H_)
#define MODELMANAGER_H_

#pragma once

extern "C"
{
#include "../raylib/raylib.h"
}

typedef int ModelID;
namespace ModelManager
{
    void Preload(const char* const aPath);
    ModelID GetModelID(const char* const aPath);
    Model* GetModel(ModelID anId);

    void Terminate();
}

#endif // MODELMANAGER_H_
