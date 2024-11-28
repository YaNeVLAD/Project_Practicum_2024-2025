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
		damage += 5;
	}
}

void MagicCharge::Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform)
{
	Entity& projectile = entityManager.CreateEntity(EntityType::Projectile);
	projectile.AddComponent<TransformComponent>(
		playerTransform->x + 16, playerTransform->y + 32, 0.0f, 0.0f
	);
	projectile.AddComponent<LifetimeComponent>(1.25f);
	projectile.AddComponent<DrawableComponent>(mFrames[0], sf::Vector2f(1.5, 1.5));
	projectile.AddComponent<AnimationComponent>(mFrames, 0.3f, true, -1.0f, true);
	projectile.AddComponent<RotationComponent>();

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	projectile.AddComponent<CollisionComponent>(std::move(collisionShape));

	projectile.AddComponent<HomingProjectileComponent>(projectileSpeed);

	projectile.AddComponent<DamageComponent>(damage, Enemy);
}

void MagicCharge::LoadTextures()
{
	mFrames = TextureManager::GetTextures("assets/weapon/Charge.png", 64, 64);
}