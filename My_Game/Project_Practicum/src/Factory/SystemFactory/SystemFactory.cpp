
#include "SystemFactory.h"

#include "../../Manager/System/SystemManager.h"

void SystemFactory::InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera)
{
    systemManager.AddSystem<InputSystem>();
    systemManager.AddSystem<WeaponSystem>();
    systemManager.AddSystem<MovementSystem>();
    systemManager.AddSystem<CollisionSystem>();
    systemManager.AddSystem<LifetimeSystem>(camera);
    systemManager.AddSystem<HomingProjectileSystem>();
    systemManager.AddSystem<OrbitalProjectileSystem>();
    systemManager.AddSystem<SpawnSystem>(camera, 1.0f);
    
    systemManager.AddSystem<RenderSystem>(window);
    systemManager.AddSystem<CameraSystem>(camera);
    systemManager.AddSystem<InterfaceSystem>(window, camera);
}