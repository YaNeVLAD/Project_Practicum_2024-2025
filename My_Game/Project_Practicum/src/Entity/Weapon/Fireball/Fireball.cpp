#include "Fireball.h"

#include "../../../Manager/Texture/TextureManager.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"

void Fireball::Upgrade(int level)
{
	if (mLevel + 1 <= MAX_LEVELS)
	{
		mLevel++;
		damage += 10;
		mTrailDamage += 5;
		projectileSpeed += 50;
		mScale += { 0.2f, 0.2f };
	}
}

void Fireball::Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform)
{
	auto& fireball = entityManager.CreateEntity(Projectile);

	fireball.AddComponent<TransformComponent>(
		playerTransform->x, playerTransform->y,
		projectileSpeed * playerTransform->lastDirection.x,
		projectileSpeed * playerTransform->lastDirection.y
	);

	fireball.AddComponent<DrawableComponent>(mFrames[0], mScale);
	fireball.AddComponent<AnimationComponent>(mFrames, 0.2f, true, -1.0f, true);

	fireball.AddComponent<LifetimeComponent>(1.8f);

	auto collisionShape = std::make_unique<sf::RectangleShape>(
		sf::Vector2f(32.0f * mScale.x, 32.0f * mScale.y)
	);
	collisionShape->setOrigin(
		16.0f * mScale.x, 16.0f * mScale.y
	);

	fireball.AddComponent<CollisionComponent>(std::move(collisionShape));

	fireball.AddComponent<DamageComponent>(damage, Enemy);

	fireball.AddComponent<TrailComponent>(mTrailDamage, 0.15f);
}

void Fireball::LoadTextures()
{
	mFrames = TextureManager::GetTextures("assets/weapon/Fireball.png", 64, 64);
}
