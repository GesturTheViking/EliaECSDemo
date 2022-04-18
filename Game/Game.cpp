#include "Game.h"

#include <stdlib.h>
#include <stdio.h>

extern "C"
{
#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
}

#include "../ECS/EntityService.h"
#include "../ECS/ComponentList.h"
#include "../ECS/Components.h"

#include "../ECS/MovementSystem.h"
#include "../ECS/RenderSystem.h"

#include "ModelManager.h"



struct GameState
{
    EntityService myEntityService;

    ComponentList<TransformComponent> myTransformComponents;
    ComponentList<MovementComponent> myMovementComponents;
    ComponentList<ModelComponent> myModelComponents;

    Entity mySpawnedEntities[MAX_ENTITIES];
    Entity mySpawnedEntitiesCount;
} gGameState;



void Game::Init()
{
    gGameState.mySpawnedEntitiesCount = 0;

    ModelManager::Preload("assets/banana.obj");
    ModelManager::Preload("assets/donut.obj");

    AddEntities(1);
}

void Game::Update()
{
    Systems::MovementUpdate(&gGameState.myTransformComponents, &gGameState.myMovementComponents);
    Systems::Render(&gGameState.myTransformComponents, &gGameState.myModelComponents);
}

void Game::Terminate()
{
    ModelManager::Terminate();
}

void Game::AddEntities(uint32_t aCount)
{
    aCount = Clamp(aCount, 0, MAX_ENTITIES - (uint32_t)gGameState.myEntityService.Count());

    for (uint32_t i = 0; i < aCount; ++i)
    {
        const Entity e = gGameState.myEntityService.GetEntity();
        gGameState.mySpawnedEntities[gGameState.mySpawnedEntitiesCount++] = e;

        const float randomPositionX = (float)GetRandomValue(-25,25);
        const float randomPositionY = (float)GetRandomValue(0,50);
        const float randomPositionZ = (float)GetRandomValue(-25,25);

        gGameState.myTransformComponents.AddComponent(e).myPosition = { randomPositionX, randomPositionY, randomPositionZ };

        const float randomVelocityX = (float)GetRandomValue(0, 10);
        const float randomVelocityY = (float)GetRandomValue(0, 10);
        const float randomVelocityZ = (float)GetRandomValue(0, 10);

        gGameState.myMovementComponents.AddComponent(e).myVelocity = { randomVelocityX, randomVelocityY, randomVelocityZ };

        ModelComponent& mdlComp = gGameState.myModelComponents.AddComponent(e);
        mdlComp.myColor = { (uint8_t)GetRandomValue(0, 255), (uint8_t)GetRandomValue(0, 255), (uint8_t)GetRandomValue(0, 255), 255 };

        int randModel = GetRandomValue(0, 1);
        mdlComp.myModel = ModelManager::GetModelID(randModel ? "assets/banana.obj" : "assets/donut.obj");
        mdlComp.myScale = randModel ? 1.0f : 50.0f;
    }
}

void Game::RemoveEntities(uint32_t aCount)
{
    aCount = Clamp(aCount, 0, gGameState.mySpawnedEntitiesCount);

    for (uint32_t i = 0; i < aCount; ++i)
    {
        const Entity e = gGameState.mySpawnedEntities[--gGameState.mySpawnedEntitiesCount];

        gGameState.myTransformComponents.RemoveComponent(e);
        gGameState.myMovementComponents.RemoveComponent(e);
        gGameState.myModelComponents.RemoveComponent(e);

        gGameState.myEntityService.ReturnEntity(e);
    }
}

bool Game::IsMaxEntitiesReached()
{
    return gGameState.myEntityService.Count() == MAX_ENTITIES;
}

uint32_t Game::GetEntityCount()
{
    return (uint32_t)gGameState.myEntityService.Count();
}