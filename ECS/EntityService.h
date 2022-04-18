#if !defined(ENTITYSERVICE_H_)
#define ENTITYSERVICE_H_

#pragma once

#include <stdint.h>
#include "../Utils/BitArray.h"

using Entity = uint32_t;
constexpr uint32_t MAX_ENTITIES = 768;
constexpr Entity INVALID_ENTITY = Entity(-1);

class EntityService
{
public:
	EntityService();
	~EntityService() = default;

	EntityService(const EntityService& ecs);
	EntityService(EntityService&& ecs) noexcept;
	EntityService& operator=(const EntityService& ecs);
	EntityService& operator=(EntityService&& ecs) noexcept;

	Entity GetEntity();
	void ReturnEntity(Entity anEntity);

	Entity GetParent(Entity anEntity) const;
	bool HasChildren(Entity anEntity) const;
	bool IsChild(Entity anEntity) const;
	BitArray<MAX_ENTITIES> GetChildren(Entity anEntity) const;
	void AppendChild(Entity aToBeParent, Entity aToBeChild);

	const BitArray<MAX_ENTITIES>& GetOccupiedEntities() const;
	size_t Count() const;

	void Clear();

private:
	Entity myParentLL[MAX_ENTITIES];

	Entity myAvailableEntitiesLL[MAX_ENTITIES];
	Entity myFirstAvailableEntity;

	BitArray<MAX_ENTITIES> myOccupiedEntities;
};

#endif // ENTITYSERVICE_H_