#include "WeaponSystem.h"

#include "../../entity/weapon/Weapon.h"

void WeaponSystem::Update(EntityManager& entityManager, float deltaTime)
{
    std::vector<Weapon*> weaponsToAttack;
    TransformComponent* entityTransform = nullptr;

    for (auto& entity : entityManager.GetEntitiesWithComponents<WeaponComponent, TransformComponent>())
    {
        auto weaponComponent = entity->GetComponent<WeaponComponent>();
        auto transform = entity->GetComponent<TransformComponent>();

        if (weaponComponent && transform)
        {
            entityTransform = transform;

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
                    weaponsToAttack.push_back(weapon.get());
                }
            }
        }
    }

    for (auto& weapon : weaponsToAttack)
    {
        weapon->Attack(entityManager, entityTransform);
    }
}