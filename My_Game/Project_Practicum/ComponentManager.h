#pragma once

#include <typeindex>
#include <unordered_map>

#include "ComponentPool.h"

class ComponentManager
{
public:
	template<typename ComponentType, typename... Args>
	void addComponent(Entity entity, Args&&... args)
	{
		getComponentPool<ComponentType>().addComponent(
			entity,
			std::forward<Args>(args)...
		);
	}

	template<typename ComponentType>
	const ComponentType& getComponent(Entity entity)
	{
		return getComponentPool<ComponentType>().getComponent(entity);
	}

	template<typename ComponentType>
	bool hasComponent(Entity entity)
	{
		return getComponentPool<ComponentType>().hasComponent(entity);
	}

	void onDestroyEntityComponents(Entity entity)
	{
		for (auto& [type, pool] : mComponentPools)
			pool->onDestroyEntityComponent(entity);
	}

private:
	template<typename ComponentType>
	ComponentPool<ComponentType>& getComponentPool()
	{
		std::type_index typeIndex(typeid(ComponentType));

		auto it = mComponentPools.find(typeIndex);
		if (it == mComponentPools.end())
		{
			auto newPool = std::make_unique<ComponentPool<ComponentType>>();
			it = mComponentPools.emplace(typeIndex, std::move(newPool)).first;
		}

		return static_cast<ComponentPool<ComponentType>&>(*it->second);
	}

	std::unordered_map <std::type_index, std::unique_ptr<BaseComponentPool>> mComponentPools;
};