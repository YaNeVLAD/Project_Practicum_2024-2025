#include "WeaponSystem.h"
#include "Weapon.h"
#include <iostream>

void WeaponSystem::Update(EntityManager& entityManager, float deltaTime)
{
    std::vector<Entity*> entitiesToAttack;

    for (auto& entity : entityManager.GetEntities())
    {
        auto weaponComponent = entity.GetComponent<WeaponComponent>();
        auto transform = entity.GetComponent<TransformComponent>();

        if (weaponComponent && transform)
        {
            if (weaponComponent->weapons.empty())
            {
                continue;
            }

            for (auto& weapon : weaponComponent->weapons)
            {
                if (weapon->cooldown > 0)
                {
                    weapon->cooldown -= deltaTime;
                }

                if (weapon->cooldown <= 0)
                {
                    std::cout << "Attack";
                    weapon->cooldown = weapon->fireRate;
                    entitiesToAttack.push_back(&entity);
                }
            }
        }
    }

    for (auto& entity : entitiesToAttack)
    {
        auto transform = entity->GetComponent<TransformComponent>();
        auto weaponComponent = entity->GetComponent<WeaponComponent>();

        if (transform && weaponComponent)
        {
            for (auto& weapon : weaponComponent->weapons)
            {
                auto& projectile = entityManager.CreateEntity();
                weapon->Attack(projectile, *transform);
            }
        }
    }
}