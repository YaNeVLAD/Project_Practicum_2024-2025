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
	mEntitiesToRemove.push_back(id);
}

void EntityManager::Clear()
{
	mEntities.clear();
	mEntitiesToAdd.clear();
	mEntitiesToRemove.clear();
}

void EntityManager::UpdateEntities()
{
	for (auto& entity : mEntitiesToAdd)
	{
		mEntities.push_back(std::move(entity));
	}
	mEntitiesToAdd.clear();

	for (const auto& id : mEntitiesToRemove)
	{
		auto it = std::remove_if(mEntities.begin(), mEntities.end(),
			[id](const Entity& entity) {return entity.GetId() == id; });

		if (it != mEntities.end())
		{
			mEntities.erase(it, mEntities.end());
		}
	}
	mEntitiesToRemove.clear();
}

Entity& EntityManager::CreateEntity(EntityType type)
{
	mEntitiesToAdd.emplace_back(mNextEntityId++, type);
	return mEntitiesToAdd.back();
}