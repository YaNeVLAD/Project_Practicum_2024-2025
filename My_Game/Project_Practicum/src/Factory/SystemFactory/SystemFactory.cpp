
#include "SystemFactory.h"

#include "../../Manager/System/SystemManager.h"

void SystemFactory::InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera)
{
    systemManager.AddSystem<RenderSystem>(window);
    systemManager.AddSystem<MovementSystem>();
    systemManager.AddSystem<InputSystem>();
    systemManager.AddSystem<CameraSystem>(camera);
    systemManager.AddSystem<WeaponSystem>();
    systemManager.AddSystem<LifetimeSystem>(camera);
    systemManager.AddSystem<OrbitalProjectileSystem>();
    systemManager.AddSystem<CollisionSystem>();
}
