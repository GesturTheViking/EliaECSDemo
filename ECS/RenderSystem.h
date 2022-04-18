#if !defined(RENDERSYSTEM_H_)
#define RENDERSYSTEM_H_

#pragma once

#include "ComponentList.h"
#include "Components.h"

#include "../Utils/Dictionary.h"

namespace Systems
{
    void Render(
        ComponentList<TransformComponent>* someTransformComps,
        ComponentList<ModelComponent>* someModelComps);
}

#endif // RENDERSYSTEM_H_
