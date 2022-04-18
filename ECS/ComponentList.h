#if !defined(COMPONENTLIST_H_)
#define COMPONENTLIST_H_

#pragma once

#include "EntityService.h"
#include "../Utils/BitArray.h"

template <class ComponentType>
class ComponentList
{
public:
	ComponentList();
	~ComponentList() = default;

	ComponentList(const ComponentList&) = delete;
	ComponentList(ComponentList&&) = delete;
	ComponentList& operator=(const ComponentList&) = delete;
	ComponentList& operator=(ComponentList&&) = delete;

	bool HasComponent(Entity anEntity);
	ComponentType& AddComponent(Entity anEntity);
	void RemoveComponent(Entity anEntity);
	ComponentType& GetComponent(Entity anEntity);
	const ComponentType& GetComponent(Entity anEntity) const;
	Entity GetEntityFromComponent(uint32_t componentIndex) const;

	ComponentType* GetDenseComponents();
	uint32_t GetSize();
	BitArray<MAX_ENTITIES>& GetEntitiesContainingComponent();

	bool IsActive(Entity anEntity);
	void Activate(Entity anEntity);
	void Deactivate(Entity anEntity);
	void SetActive(Entity anEntity, bool aValue = true);
	void ActivateAll();

	void SetComponentAsDefaultForAllEntities();

private:
	ComponentType myComponents[MAX_ENTITIES];
	uint32_t myComponentsSize;

	uint32_t myMapEntityToComponent[MAX_ENTITIES];
	uint32_t myMapComponentToEntity[MAX_ENTITIES];

	BitArray<MAX_ENTITIES> myEntitiesContainingComponent;
	BitArray<MAX_ENTITIES> myActiveEntities;
};

template<class ComponentType>
inline ComponentList<ComponentType>::ComponentList()
{
	myActiveEntities.SetAll();
}

template<class ComponentType>
inline bool ComponentList<ComponentType>::HasComponent(Entity anEntity)
{
	assert(anEntity < MAX_ENTITIES && "Entity out of range.");

	return myEntitiesContainingComponent.Test(anEntity);
}

template<class ComponentType>
inline ComponentType& ComponentList<ComponentType>::AddComponent(Entity anEntity)
{
	assert(anEntity < MAX_ENTITIES && "Entity out of range.");
	assert(!myEntitiesContainingComponent.Test(anEntity) && "Entity already has component.");

	myEntitiesContainingComponent.Set(anEntity);
	myActiveEntities.Set(anEntity);

	uint32_t componentIndex = myComponentsSize++;
	myComponents[componentIndex] = ComponentType();
	myMapEntityToComponent[anEntity] = componentIndex;
	myMapComponentToEntity[componentIndex] = anEntity;

	return myComponents[componentIndex];
}

template<class ComponentType>
inline void ComponentList<ComponentType>::RemoveComponent(Entity anEntity)
{
	assert(anEntity < MAX_ENTITIES && "Entity out of range.");

	myEntitiesContainingComponent.Reset(anEntity);
	myActiveEntities.Reset(anEntity);

	--myComponentsSize;
	uint32_t componentIndex = myMapEntityToComponent[anEntity];
	myComponents[componentIndex] = myComponents[myComponentsSize];
	myMapEntityToComponent[myMapComponentToEntity[myComponentsSize]] = componentIndex;
	myMapComponentToEntity[componentIndex] = myMapComponentToEntity[myComponentsSize];
}

template<class ComponentType>
inline ComponentType& ComponentList<ComponentType>::GetComponent(Entity anEntity)
{
	assert(HasComponent(anEntity) && "This entity does not yet have a component of this type.");
	assert(anEntity < MAX_ENTITIES && "Entity out of range.");

	return myComponents[myMapEntityToComponent[anEntity]];
}

template<class ComponentType>
inline const ComponentType& ComponentList<ComponentType>::GetComponent(Entity anEntity) const
{
	assert(HasComponent(anEntity) && "This entity does not yet have a component of this type.");
	assert(anEntity < MAX_ENTITIES && "Entity out of range.");

	return myComponents[myMapEntityToComponent[anEntity]];
}

template<class ComponentType>
inline Entity ComponentList<ComponentType>::GetEntityFromComponent(uint32_t componentIndex) const
{
	assert(componentIndex < myComponentsSize && "Index out of bounds.");

	return myMapComponentToEntity[componentIndex];
}

template<class ComponentType>
inline ComponentType* ComponentList<ComponentType>::GetDenseComponents()
{
	return myComponents;
}

template<class ComponentType>
inline uint32_t ComponentList<ComponentType>::GetSize()
{
	return myComponentsSize;
}

template<class ComponentType>
inline BitArray<MAX_ENTITIES>& ComponentList<ComponentType>::GetEntitiesContainingComponent()
{
	return myEntitiesContainingComponent;
}

template<class ComponentType>
inline bool ComponentList<ComponentType>::IsActive(Entity anEntity)
{
	return myEntitiesContainingComponent[anEntity] && myActiveEntities[anEntity];
}

template<class ComponentType>
inline void ComponentList<ComponentType>::Activate(Entity anEntity)
{
	myActiveEntities.Set(anEntity);
}

template<class ComponentType>
inline void ComponentList<ComponentType>::Deactivate(Entity anEntity)
{
	myActiveEntities.Reset(anEntity);
}

template<class ComponentType>
inline void ComponentList<ComponentType>::SetActive(Entity anEntity, bool aValue)
{
	if (aValue)
	{
		myActiveEntities.Set(anEntity);
	}
	else
	{
		myActiveEntities.Reset(anEntity);
	}
}

template<class ComponentType>
inline void ComponentList<ComponentType>::ActivateAll()
{
	myActiveEntities.SetAll();
}

template<class ComponentType>
inline void ComponentList<ComponentType>::SetComponentAsDefaultForAllEntities()
{
	myEntitiesContainingComponent.SetAll();
}

#endif // COMPONENTLIST_H_