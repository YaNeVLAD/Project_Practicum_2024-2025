#pragma once

#include "SystemContext.h"

class System
{
public:
	virtual ~System() = default;

	virtual void onAdded() = 0;
	virtual void update(float dt, SystemContext& context) = 0;
	virtual void onRemove() = 0;

	void onEnable(bool enabled)
	{
		mEnabled = enabled;
	}

	bool isEnabled() const
	{
		return mEnabled;
	}

private:
	bool mEnabled = true;
};