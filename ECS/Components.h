#if !defined(COMPONENTS_H_)
#define COMPONENTS_H_

#pragma once

extern "C"
{
#include "../raylib/raylib.h"
}
#include "../Game/ModelManager.h"

struct TransformComponent
{
    Vector3 myPosition = { .0f, .0f, .0f };
};

struct MovementComponent
{
    Vector3 myVelocity = { .0f, .0f, .0f };
};

struct ModelComponent
{
    ModelID myModel;
    Color myColor;
    float myScale;
};

#endif // COMPONENTS_H_
