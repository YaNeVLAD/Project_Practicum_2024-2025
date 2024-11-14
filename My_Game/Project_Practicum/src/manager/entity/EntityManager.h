#pragma once

#include "../../Entity/Entity.h"

class EntityManager
{
public:
	Entity& CreateEntity();

	std::vector<Entity>& GetEntities();

	void RemoveEntity(Entity::IdType id);

private:
	std::vector<Entity> mEntities;
	Entity::IdType mNexEntityId = 0;
};
