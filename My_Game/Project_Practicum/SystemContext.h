#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include <functional>

class SystemContext
{
public:
	SystemContext(EntityManager& entityManager, ComponentManager& componentManager)
		: mEntityManager(entityManager), mComponentManager(componentManager) {}

	template<typename ComponentType>
	bool hasComponent(Entity entity)
	{
		return mComponentManager.hasComponent(entity);
	}

	template<typename... ComponentTypes>
	std::vector<Entity> getEntitiesWithComponents()
	{
		std::vector<Entity> entities;

		auto& activeEntites = EntityManager.getActiveEntities();
		for (const auto& entity : activeEntites)
			if ((mComponentManager.hasComponent<ComponentTypes>(entity) && ...))
				entities.push_back(entity);

		return entities;
	}

private:
	EntityManager& mEntityManager;
	ComponentManager mComponentManager;
};