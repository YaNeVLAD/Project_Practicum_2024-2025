#pragma once

#include "SystemManager.h"

class ECSManager
{
public:
	ECSManager() : mContext(mEntityManager, mComponentManager) {}

	Entity createEntity()
	{
		return mEntityManager.createEntity();
	}

	void destroyEntity(Entity entity)
	{
		mComponentManager.onDestroyEntityComponents(entity);
		mEntityManager.destroyEntity(entity);
	}

	template<typename ComponentType, typename... Args>
	void addComponent(Entity entity, Args&&... args)
	{
		mComponentManager.addComponent<ComponentType>(entity, std::forward(args)...);
	}

	template<typename SystemType, typename... Args>
	void addSystem(Args&&... args)
	{
		mSystemManager.addSystem<SystemType>(std::forward(args)...);
	}

	template<typename SystemType>
	void removeSystem()
	{
		mSystemManager.removeSystem<SystemType>();
	}

	template<typename SystemType>
	void hasSystem() const
	{
		return mSystemManager.hasSystem<SystemType>();
	}

	template<typename SystemType>
	void enableSystem(bool enabled)
	{
		mSystemManager.enableSystem<SystemType>(enabled);
	}

	template<typename SystemType>
	void updateSystems(float dt)
	{
		mSystemManager.updateSystems(dt, mContext);
	}

private:
	EntityManager mEntityManager;
	ComponentManager mComponentManager;
	SystemManager mSystemManager;
	SystemContext mContext;
};