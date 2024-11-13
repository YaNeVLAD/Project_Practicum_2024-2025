#include "EntityManager.h"

std::vector<Entity>& EntityManager::GetEntities()
{
	return mEntities;
}

Entity& EntityManager::CreateEntity()
{
	mEntities.emplace_back(mNexEntityId++);
	return mEntities.back();
}