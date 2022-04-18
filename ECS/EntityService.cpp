#include "EntityService.h"

#include <assert.h>

EntityService::EntityService()
	: myFirstAvailableEntity(0)
	, myAvailableEntitiesLL{(Entity)-1}
{
	for (Entity ent = 0; ent < MAX_ENTITIES; ++ent)
	{
		myAvailableEntitiesLL[ent] = ent + 1;
		myParentLL[ent] = (Entity)-1;
	}
}

EntityService::EntityService(const EntityService& ecs)
	: myFirstAvailableEntity(0)
	, myAvailableEntitiesLL{ (Entity)-1 }
{
	memcpy(this, &ecs, sizeof(EntityService));
}

EntityService::EntityService(EntityService&& ecs) noexcept
	: myFirstAvailableEntity(0)
	, myAvailableEntitiesLL{ (Entity)-1 }
{
	memcpy(this, &ecs, sizeof(EntityService));
}

EntityService& EntityService::operator=(const EntityService& ecs)
{
	memcpy(this, &ecs, sizeof(EntityService));
	
	return *this;
}

EntityService& EntityService::operator=(EntityService&& ecs) noexcept
{
	memcpy(this, &ecs, sizeof(EntityService));

	return *this;
}

Entity EntityService::GetEntity()
{
	assert(myFirstAvailableEntity < MAX_ENTITIES && "There are no available entities.");

	Entity newEntity = myFirstAvailableEntity;
	myFirstAvailableEntity = myAvailableEntitiesLL[myFirstAvailableEntity];
	myAvailableEntitiesLL[newEntity] = (Entity)-1;
	myParentLL[newEntity] = (Entity)-1;
	myOccupiedEntities.Set(newEntity);

	return newEntity;
}

void EntityService::ReturnEntity(Entity anEntity)
{
	assert(anEntity < MAX_ENTITIES && "Entity out of range.");
	assert(myAvailableEntitiesLL[anEntity] >= MAX_ENTITIES && "Attempting to return already available entity.");

	if (myAvailableEntitiesLL[anEntity] >= MAX_ENTITIES)
	{
		myOccupiedEntities.Reset(anEntity);
		myAvailableEntitiesLL[anEntity] = myFirstAvailableEntity;
		myFirstAvailableEntity = anEntity;
	}
}

Entity EntityService::GetParent(Entity anEntity) const
{
	assert(anEntity < MAX_ENTITIES && "Entity out of range.");

	return myParentLL[anEntity];
}

bool EntityService::HasChildren(Entity anEntity) const
{
	assert(anEntity < MAX_ENTITIES && "Entity out of range.");

	for (Entity e = 0U; e < MAX_ENTITIES; ++e)
	{
		if (myParentLL[e] == anEntity)
		{
			return true;
		}
	}

	return false;
}

bool EntityService::IsChild(Entity anEntity) const
{
	assert(anEntity < MAX_ENTITIES && "Entity out of range.");

	return myParentLL[anEntity] < MAX_ENTITIES;
}

BitArray<MAX_ENTITIES> EntityService::GetChildren(Entity anEntity) const
{
	assert(anEntity < MAX_ENTITIES && "Entity out of range.");

	BitArray<MAX_ENTITIES> entities;

	for (Entity e = 0U; e < MAX_ENTITIES; ++e)
	{
		if (myParentLL[e] == anEntity && myOccupiedEntities.Test(e))
		{
			entities.Set(e);
		}
	}

	return entities;
}

void EntityService::AppendChild(Entity aToBeParent, Entity aToBeChild)
{
	assert(aToBeChild < MAX_ENTITIES && "Entity out of range.");

	myParentLL[aToBeChild] = aToBeParent;
}

const BitArray<MAX_ENTITIES>& EntityService::GetOccupiedEntities() const
{
	return myOccupiedEntities;
}

size_t EntityService::Count() const
{
	return myOccupiedEntities.Count();
}

void EntityService::Clear()
{
	myFirstAvailableEntity = 0;

	for (Entity ent = 0; ent < MAX_ENTITIES; ++ent)
	{
		myAvailableEntitiesLL[ent] = ent + 1;
		myParentLL[ent] = (Entity)-1;
	}

	myOccupiedEntities.ResetAll();
}