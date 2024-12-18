#include "WeaponSystem.h"

#include "../../entity/weapon/Weapon.h"

void WeaponSystem::Update(EntityManager& entityManager, float deltaTime)
{
    auto player = entityManager.GetEntitiesWithType(Player);
    if (player.empty())
    {
        return;
    }

    auto playerTransform = player.front()->GetComponent<TransformComponent>();
    if (playerTransform == nullptr)
    {
        return;
    }

    std::vector<Weapon*> weaponsToAttack;

    for (auto& entity : entityManager.GetEntitiesWithComponents<WeaponComponent, TransformComponent>())
    {
        auto weaponComponent = entity->GetComponent<WeaponComponent>();
        auto transform = entity->GetComponent<TransformComponent>();

        if (weaponComponent != nullptr && transform != nullptr)
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
                    weapon->cooldown = weapon->fireRate;
                    weapon->Attack(entityManager, transform, playerTransform);
                }
            }
        }
    }
}