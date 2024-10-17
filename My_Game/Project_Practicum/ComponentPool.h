#pragma once

#include <stdexcept>
#include <vector>
#include <memory>

#include "Components.h"
#include "ComponentPoolExceptions.h"

class BaseComponentPool
{
public:
	virtual ~BaseComponentPool() = default;
	virtual void onDestroyEntityComponent(Entity entity) = 0;
	virtual bool hasComponent(Entity entity) = 0;
};

template<typename ComponentType>
class ComponentPool : public BaseComponentPool
{
public:
	template<typename... Args>
	void addComponent(Entity entity, Args&&... args)
	{
		if (entity >= pool.size())
			pool.resize(entity + 1);
		pool[entity] = Component(std::forward<Args>(args)...);
	}

	void onDestroyEntityComponent(Entity entity) override
	{
		if (entity < pool.size())
			pool[entity].reset();
	}

	const ComponentType& getComponent(Entity entity) const
	{
		if (entity >= pool.size() || pool[entity] == nullptr)
			throw ComponentEntityNotFoundException(entity, typeid(ComponentType).name());

		return *pool[entity];
	}

	bool hasComponent(Entity entity) const override
	{
		return entity < pool.size() && pool[entity] != nullptr;
	}
	
private:
	std::vector<ComponentType> pool;
};