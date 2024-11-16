
#include "SystemFactory.h"

#include "../../Manager/System/SystemManager.h"

void SystemFactory::InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera)
{
    systemManager.AddSystem<MovementSystem>();
    systemManager.AddSystem<InputSystem>();
    systemManager.AddSystem<WeaponSystem>();
    systemManager.AddSystem<LifetimeSystem>(camera);
    systemManager.AddSystem<OrbitalProjectileSystem>();
    systemManager.AddSystem<SpawnSystem>(camera, 1.0f);
    systemManager.AddSystem<CollisionSystem>();
    systemManager.AddSystem<HomingProjectileSystem>();
    
    systemManager.AddSystem<CameraSystem>(camera);
    systemManager.AddSystem<RenderSystem>(window);
}