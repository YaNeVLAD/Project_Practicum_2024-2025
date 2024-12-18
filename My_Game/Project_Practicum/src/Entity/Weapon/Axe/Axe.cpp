#include "Axe.h"

#include "../../Entity.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../Manager/Texture/TextureManager.h"

void Axe::Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform)
{
    Entity& projectile = entityManager.CreateEntity(EntityType::Projectile);

    float angle = 75.f;

    float angleRad = angle * 3.14159f / 180.f;

    float vx = projectileSpeed * std::cos(angleRad) * playerTransform->lastDirection.x;
    float vy = -projectileSpeed * std::sin(angleRad);

    projectile.AddComponent<TransformComponent>(
        playerTransform->GetPosition(),
        sf::Vector2f(vx, vy)
    );

    projectile.AddComponent<ParaboloidProjectileComponent>(mGravity);

    projectile.AddComponent<DamageComponent>(damage, 0.1f, EntityType::Enemy);

    auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(64, 64));
    collisionShape->setOrigin(32, 32);
    projectile.AddComponent<CollisionComponent>(std::move(collisionShape));

    projectile.AddComponent<DrawableComponent>(mFrames[0]);
    projectile.AddComponent<AnimationComponent>(0.1f);

    auto animation = projectile.GetComponent<AnimationComponent>();
    animation->AddAnimation(AnimationComponent::ATTACK, mFrames);
    animation->SetState(AnimationComponent::ATTACK);
}

void Axe::Upgrade(int level)
{
    if (CanUpgrade())
    {
        mLevel++;
        damage += 2;
        fireRate -= 0.1f;
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

void Axe::LoadTextures()
{
    mFrames = TextureManager::GetTextures("assets/weapon/Axe.png", 64, 64);
}
