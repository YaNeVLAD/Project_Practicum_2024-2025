#include "Fireball.h"

#include "../../../Manager/Texture/TextureManager.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"

void Fireball::Upgrade(int level)
{
	if (CanUpgrade())
	{
		mLevel++;
		damage += 7;
		mTrailDamage += 2;
		projectileSpeed += 50;
		mScale += { 0.2f, 0.2f };
	}
}

void Fireball::Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform)
{
    auto& fireball = entityManager.CreateEntity(Projectile);

    sf::Vector2f direction = playerTransform->lastDirection;
    
    if (direction.x != 0 && direction.y != 0)
    {
        direction = direction / std::sqrt(2.0f);
    }

    fireball.AddComponent<TransformComponent>(
        playerTransform->GetPosition(),
        projectileSpeed * direction
    );

    fireball.AddComponent<DrawableComponent>(mFrames->at(0), mScale);
    fireball.AddComponent<AnimationComponent>(0.3f, true);
    auto animation = fireball.GetComponent<AnimationComponent>();
    animation->AddAnimation(AnimationComponent::ATTACK, mFrames);
    animation->SetState(AnimationComponent::ATTACK);

    fireball.AddComponent<LifetimeComponent>(2.f);

    auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32.0f * mScale.x, 32.0f * mScale.y));
    collisionShape->setOrigin(16.0f * mScale.x, 16.0f * mScale.y);
    fireball.AddComponent<CollisionComponent>(std::move(collisionShape));

    fireball.AddComponent<DamageComponent>(damage, 0.5f, Enemy);
    fireball.AddComponent<TrailComponent>(mTrailDamage, 0.1f);
}

void Fireball::LoadTextures()
{
	mFrames = TextureManager::GetTextures("assets/weapon/Fireball.png", 64, 64);
}