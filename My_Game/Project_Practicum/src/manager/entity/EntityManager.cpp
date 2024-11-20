#include "EntityManager.h"

std::vector<Entity>& EntityManager::GetEntities()
{
	return mEntities;
}

std::vector<Entity*> EntityManager::GetEntitiesWithType(EntityType type)
{
	std::vector<Entity*> result;
	for (Entity& entity : mEntities)
	{
		if (entity.GetType() == type)
		{
			result.push_back(&entity);
		}
	}

	return result;
}

void EntityManager::RemoveEntity(Entity::IdType id)
{
	auto it = std::remove_if(mEntities.begin(), mEntities.end(),
		[id](const Entity& entity) {return entity.GetId() == id; });

	if (it != mEntities.end())
	{
		mEntities.erase(it, mEntities.end());
	}
}

Entity& EntityManager::CreateEntity(EntityType type)
{
	mEntities.emplace_back(mNextEntityId++, type);
	return mEntities.back();
}