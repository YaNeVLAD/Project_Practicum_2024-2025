#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID()
{
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr std::size_t MAX_COMPONENTS = 32;

using ComponentBitSet = std::bitset<MAX_COMPONENTS>;
using ComponentArray = std::array<Component*, MAX_COMPONENTS>;

class Component
{
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
public:
	void update()
	{
		for (auto& component : mComponents)
			component->update();
		for (auto& component : mComponents)
			component->draw();
	}

	void draw() {}
	bool isActive() const { return mActive; }
	void destroy() { mActive = false; }

	template<typename T>
	bool hasComponent() const
	{
		return ComponentBitSet[getComponentTypeID<T>()];
	}

	template<typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* component(new T(std::forward<TArgs>(mArgs)...));
		component->entity = this;
		std::unique_ptr<Component> uPtr{ component };
		mComponents.emplace_back(std::move(uPtr));

		mComponentArray[getComponentTypeID<T>()] = component;
		mComponentBitSet[getComponentTypeID<T>()] = true;

		component->init();
		return *component;
	}

	template<typename T>
	T& getComponent() const
	{
		auto ptr(mComponentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

private:
	bool mActive = true;
	std::vector<std::unique_ptr<Component>> mComponents;

	ComponentArray mComponentArray;
	ComponentBitSet mComponentBitSet;
};

class Manager
{
public:
	void update()
	{
		for (auto& entity : mEntities)
			entity->update();
	}

	void draw()
	{
		for (auto& entity : mEntities)
			entity->draw();
	}

	void refresh()
	{
		mEntities.erase(std::remove_if(std::begin(mEntities), std::end(mEntities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(mEntities));
	}

	Entity& addEntity()
	{
		Entity* entity = new Entity();
		std::unique_ptr<Entity> uPtr{ entity };
		mEntities.emplace_back(std::move(uPtr));

		return *entity;
	}

private:
	std::vector<std::unique_ptr<Entity>> mEntities;
};