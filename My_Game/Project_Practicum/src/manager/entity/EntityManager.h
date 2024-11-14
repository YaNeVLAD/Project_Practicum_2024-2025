#pragma once

#include "../../Entity/Entity.h"

class EntityManager
{
public:
	Entity& CreateEntity();

	std::vector<Entity>& GetEntities();

	void RemoveEntity(Entity::IdType id);

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