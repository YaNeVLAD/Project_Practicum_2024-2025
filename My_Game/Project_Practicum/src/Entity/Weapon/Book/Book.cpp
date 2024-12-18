#include "Book.h"

#include "../../../Manager/Texture/TextureManager.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"
#include <iostream>

void Book::Upgrade(int level)
{
	if (CanUpgrade())
	{
		mLevel++;
		damage += 2;
		fireRate -= 1.0f;
	}
}

void Book::Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform)
{
	Entity& projectile = entityManager.CreateEntity(EntityType::Projectile);

	projectile.AddComponent<TransformComponent>(
		playerTransform->GetPosition()
	);
	projectile.AddComponent<OrbitalProjectileComponent>(125.0f, 2.0f, playerTransform);
	projectile.AddComponent<LifetimeComponent>(mLifetime);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(35.625, 48));
	collisionShape->setOrigin(17.8125, 24);
	projectile.AddComponent<CollisionComponent>(std::move(collisionShape));

	projectile.AddComponent<DrawableComponent>(mFrames[0], sf::Vector2f(0.375, 0.375));

	projectile.AddComponent<DamageComponent>(damage, 0.1f, Enemy);
}

void Book::LoadTextures()
{
	mFrames = TextureManager::GetTextures("assets/weapon/Book.png", 95, 128);
}