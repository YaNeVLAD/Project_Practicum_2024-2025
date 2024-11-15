#pragma once

#include "../../System/System.h"
#include "../../System/Spawn/SpawnSystem.h"
#include "../../System/Input/InputSystem.h"
#include "../../System/Render/RenderSystem.h"
#include "../../System/Camera/CameraSystem.h"
#include "../../System/Weapon/WeaponSystem.h"
#include "../../System/Movement/MovementSystem.h"
#include "../../System/Lifetime/LifetimeSystem.h"
#include "../../System/Collision/CollisionSystem.h"
#include "../../System/OrbitalProjectile/OrbitalProjectileSystem.h"

class SystemManager
{
public:
	template<typename SystemType, typename... Args>
	void AddSystem(Args&&... args)
	{
		auto system = std::make_unique<SystemType>(std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<IRenderSystem, SystemType>)
        {
            mRenderSystems.push_back(std::move(system));
        }
        else if constexpr (std::is_base_of_v<IUpdateSystem, SystemType>)
        {
            mUpdateSystems.push_back(std::move(system));
        }

        mSystems.push_back(std::move(system));
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

	std::vector<std::unique_ptr<System>>& GetAllSystems();

    std::vector<std::unique_ptr<IUpdateSystem>>& GetUpdateSystems();

    std::vector<std::unique_ptr<IRenderSystem>>& GetRenderSystems();

private:
	std::vector<std::unique_ptr<System>> mSystems;
    std::vector<std::unique_ptr<IUpdateSystem>> mUpdateSystems;
    std::vector<std::unique_ptr<IRenderSystem>> mRenderSystems;
};