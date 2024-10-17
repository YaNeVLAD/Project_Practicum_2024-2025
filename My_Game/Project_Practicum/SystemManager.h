#pragma once

#include <list>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "System.h"
#include "SystemExceptions.h"

class SystemManager
{
public:
	template<typename SystemType, typename... Args>
	void addSystem(Args&&... args)
	{
		std::type_index typeIndex(SystemType);
		if (mSystemLookup.find(typeIndex) != mSystemLookup.end())
			throw SystemAlreadyAddedException(typeid(SystemType).name());

		std::unique_ptr<System> system = std::make_unique<SystemType>(std::forward<Args>(args)...);

		system->onAdded();
		mSystems.emplace_back(std::move(system));
		mSystemLookup[typeIndex] = mSystems.size() - 1;
	}

	template<typename SystemType>
	void removeSystem(System system)
	{
		std::type_index typeIndex(SystemType);
		auto it = mSystemLookup.find(typeIndex);

		if (it == mSystemLookup.end())
			throw SystemNotFoundException(typeid(SystemType).name());

		mSystems[it->second]->onRemoved();
		mSystems.erase(mSystems.begin() + it->second);
		mSystemLookup.erase(it);
	}

	template<typename SystemType>
	SystemType& getSystem()
	{
		std::type_index typeIndex(SystemType);
		auto it = mSystemLookup.find(typeIndex);

		if (it == mSystemLookup.end())
			throw SystemNotFoundException(typeid(SystemType).name());

		return *static_cast<SystemType*>(mSystems[it->second].get());
	}

	template<typename SystemType>
	bool hasSystem()
	{
		std::type_index typeIndex(SystemType);
		return mSystemLookup.find(typeIndex) != mSystemLookup.end();
	}

	template<typename SystemType>
	void enableSystem(bool enabled)
	{
		auto system = getSystem<SystemType>();

		system->enabled(enabled);
	}

	void updateSystems(float dt, SystemContext& context) const
	{
		for (auto& system : mSystems)
			if (system->isEnabled())
				system->update(dt, context);
	}

private:
	std::vector<std::unique_ptr<System>> mSystems;
	std::unordered_map<std::type_index, size_t> mSystemLookup;
};