#if !defined(GAME_H_)
#define GAME_H_

#pragma once

#include <stdint.h>

namespace Game
{
    void Init();
    void Update();
    void Terminate();

    void AddEntities(uint32_t aCount);
    void RemoveEntities(uint32_t aCount);

    bool IsMaxEntitiesReached();
    uint32_t GetEntityCount();
}

#endif // GAME_H_
