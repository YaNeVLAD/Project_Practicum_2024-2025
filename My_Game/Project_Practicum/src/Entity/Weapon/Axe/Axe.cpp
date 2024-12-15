#include "Axe.h"

#include "../../Entity.h"
#include "../../../Manager/Entity/EntityManager.h"
#include <iostream>

void Axe::Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform)
{
    Entity& projectile = entityManager.CreateEntity(EntityType::Projectile);

    float angle = 45.f;

    float angleRad = angle * 3.14159f / 180.f;

    float vx = projectileSpeed * std::cos(angleRad) * playerTransform->lastDirection.x;
    float vy = -projectileSpeed * std::sin(angleRad);

    projectile.AddComponent<TransformComponent>(
        playerTransform->GetPosition(),
        sf::Vector2f(vx, vy)
    );

    projectile.AddComponent<ParaboloidProjectileComponent>(mGravity);

    projectile.AddComponent<DamageComponent>(damage, 0.3f, EntityType::Enemy);

    auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
    collisionShape->setOrigin(16, 16);
    projectile.AddComponent<CollisionComponent>(std::move(collisionShape));

    projectile.AddComponent<DrawableComponent>(32, 32, sf::Color::Cyan);
}

void Axe::Upgrade(int level)
{
    if (CanUpgrade())
    {
        mLevel++;
        damage += 5;
        fireRate -= 0.2f;
    }
}

std::string Axe::GetName()
{
    return WEAPON_NAME;
}

int Axe::GetLevel()
{
    return mLevel;
}

bool Axe::CanUpgrade()
{
    return mLevel < MAX_LEVELS;
}