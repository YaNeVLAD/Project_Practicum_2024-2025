#pragma once

#include <queue>
#include <unordered_set>

#include "Entity.h"

class EntityManager
{
public:
	Entity createEntity();
	void destroyEntity(Entity entity);

	const std::unordered_set<Entity>& getActiveEntities() const
	{
		return mActiveEntities;
	}

private:
	std::queue<Entity> mFreeEntities;
	std::unordered_set<Entity> mActiveEntities;
	Entity mNextEntity = 0;
};