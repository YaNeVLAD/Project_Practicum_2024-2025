#include "MagicCharge.h"

#include "../../../Manager/Texture/TextureManager.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"
#include <iostream>

void MagicCharge::Upgrade(int level)
{
	if (mLevel + 1 <= MAX_LEVELS)
	{
		mLevel++;
		damage += 2;
	}
}

void MagicCharge::Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform)
{
	Entity& projectile = entityManager.CreateEntity(EntityType::Projectile);
	projectile.AddComponent<TransformComponent>(
		playerTransform->GetPosition() + sf::Vector2f(16, 32)
	);
	projectile.AddComponent<LifetimeComponent>(1.25f);

	projectile.AddComponent<DrawableComponent>(mFrames[0], sf::Vector2f(1.5, 1.5));
	projectile.AddComponent<AnimationComponent>(0.3f, true);
	auto animation = projectile.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::ATTACK, mFrames);
	animation->SetState(AnimationComponent::ATTACK);
	
	projectile.AddComponent<RotationComponent>();

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	projectile.AddComponent<CollisionComponent>(std::move(collisionShape));

	projectile.AddComponent<HomingProjectileComponent>(projectileSpeed, EntityType::Enemy);

	projectile.AddComponent<DamageComponent>(damage, 0.25f, Enemy);
}

void MagicCharge::LoadTextures()
{
	mFrames = TextureManager::GetTextures("assets/weapon/Charge.png", 64, 64);
}