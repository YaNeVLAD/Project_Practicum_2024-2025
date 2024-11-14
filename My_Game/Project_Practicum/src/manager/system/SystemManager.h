#pragma once

#include "../../System/System.h"
#include "../../System/Movement/MovementSystem.h"
#include "../../System/Render/RenderSystem.h"
#include "../../System/Input/InputSystem.h"
#include "../../System/Camera/CameraSystem.h"
#include "../../System/Weapon/WeaponSystem.h"
#include "../../System/Projectile/LifetimeSystem.h"
#include "../../System/OrbitalProjectile/OrbitalProjectileSystem.h"

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