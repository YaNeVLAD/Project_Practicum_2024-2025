#pragma once

#include "System.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "InputSystem.h"
#include "CameraSystem.h"
#include "WeaponSystem.h"
#include "ProjectileSystem.h"

class SystemManager
{
public:
	template<typename SystemType, typename... Args>
	void AddSystem(Args&&... args)
	{
		mSystems.push_back(std::make_unique<SystemType>(std::forward<Args>(args)...));
	}

	std::vector<std::unique_ptr<System>>& GetSystems();

private:
	std::vector<std::unique_ptr<System>> mSystems;
};