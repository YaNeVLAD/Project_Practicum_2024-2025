#include "EntityManager.h"
#include "EntityExceptions.h"

Entity EntityManager::createEntity()
{
	Entity entity;
	if (mFreeEntities.empty())
	{
		entity = mFreeEntities.front();
		mFreeEntities.pop();
	}
	else
	{
		entity = mNextEntity++;
	}

	mActiveEntities.insert(entity);
	return entity;
}

void EntityManager::destroyEntity(Entity entity)
{
	if (mActiveEntities.find(entity) == mActiveEntities.end())
		throw EntityOutOfBoundsException(entity);

	mFreeEntities.push(entity);
	mActiveEntities.erase(entity);
}