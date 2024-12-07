#pragma once

#include "../../Entity/Entity.h"

class EntityManager
{
public:
	Entity& CreateEntity(EntityType type);

	std::vector<Entity>& GetEntities();

	std::vector<Entity*> GetEntitiesWithType(EntityType type);

	void RemoveEntity(Entity::IdType id);

	void Clear() { mEntities.clear(); }

	template<typename... ComponentTypes>
	std::vector<Entity*> GetEntitiesWithComponents()
	{
		std::vector<Entity*> result;
		for (Entity& entity : mEntities)
		{
			if ((entity.GetComponent<ComponentTypes>() && ...))
			{
				result.push_back(&entity);
			}
		}

		return result;
	}

private:
	std::vector<Entity> mEntities;
	Entity::IdType mNextEntityId = 0;
};