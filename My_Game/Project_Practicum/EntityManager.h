#pragma once

#include "Entity.h"

class EntityManager
{
public:
	Entity& CreateEntity();

	std::vector<Entity>& GetEntities();

private:
	std::vector<Entity> mEntities;
	Entity::IdType mNexEntityId = 0;
};
