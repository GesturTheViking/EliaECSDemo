#if !defined(MOVEMENTSYSTEM_H_)
#define MOVEMENTSYSTEM_H_

#pragma once

#include "ComponentList.h"
#include "Components.h"

namespace Systems
{
    void MovementUpdate(ComponentList<TransformComponent>* someTransformComps, ComponentList<MovementComponent>* someMovementComps);
}

#endif // MOVEMENTSYSTEM_H_