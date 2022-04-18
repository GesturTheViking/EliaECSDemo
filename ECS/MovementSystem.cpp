#include "MovementSystem.h"

extern "C"
{
#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
}

void Systems::MovementUpdate(ComponentList<TransformComponent>* someTransformComps, ComponentList<MovementComponent>* someMovementComps)
{
    MovementComponent* movList = someMovementComps->GetDenseComponents();
    const float dt = GetFrameTime();

    const uint32_t movCount = someMovementComps->GetSize();
    for (uint32_t compIndex = 0U; compIndex < movCount; ++compIndex)
    {
        Vector3& vel = movList[compIndex].myVelocity;
        Vector3& pos = someTransformComps->GetComponent(someMovementComps->GetEntityFromComponent(compIndex)).myPosition;

        pos.x += vel.x * dt;
        pos.y += vel.y * dt;
        pos.z += vel.z * dt;

        if (pos.x > 25.f || pos.x < -25.f)
        {
            pos.x = Clamp(pos.x, -25.f, 25.f);
            vel.x *= -1.0f;
        }
        if (pos.y > 50.f || pos.y < 0.f)
        {
            pos.y = Clamp(pos.y, 0.f, 50.f);
            vel.y *= -1.0f;
        }
        if (pos.z > 25.f || pos.z < -25.f)
        {
            pos.z = Clamp(pos.z, -25.f, 25.f);
            vel.z *= -1.0f;
        }
    }
}