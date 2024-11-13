#pragma once

#include "../../system/System.h"
#include "../../system/movement/MovementSystem.h"
#include "../../system/render/RenderSystem.h"
#include "../../system/input/InputSystem.h"
#include "../../system/camera/CameraSystem.h"
#include "../../system/weapon/WeaponSystem.h"
#include "../../system/projectile/ProjectileSystem.h"

class SystemManager
{
public:
	template<typename SystemType, typename... Args>
	void AddSystem(Args&&... args)
	{
		mSystems.push_back(std::make_unique<SystemType>(std::forward<Args>(args)...));
	}

    template<typename SystemType>
    SystemType* GetSystem()
    {
        for (auto& system : mSystems)
        {
            if (auto* castedSystem = dynamic_cast<SystemType*>(system.get()))
            {
                return castedSystem;
            }
        }
        return nullptr;
    }

	std::vector<std::unique_ptr<System>>& GetSystems();

private:
	std::vector<std::unique_ptr<System>> mSystems;
};