#include "Book.h"

#include "../../../Manager/Texture/TextureManager.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"
#include <iostream>

void Book::Upgrade(int level)
{
	if (mLevel + 1 <= MAX_LEVELS)
	{
		mLevel++;
		std::cout << WEAPON_NAME << " " << "upgraded to level" << " " << mLevel << std::endl;
	}
}

void Book::Attack(EntityManager& entityManager, TransformComponent* parentTransform)
{
	Entity& projectile = entityManager.CreateEntity(EntityType::Projectile);

	projectile.AddComponent<TransformComponent>(
		parentTransform->x, parentTransform->y, 0.0f, 0.0f
	);
	projectile.AddComponent<OrbitalProjectileComponent>(100.0f, 2.0f, parentTransform);
	projectile.AddComponent<LifetimeComponent>(4.0f);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(35.625, 48));
	collisionShape->setOrigin(17.8125, 24);
	projectile.AddComponent<CollisionComponent>(std::move(collisionShape));

	projectile.AddComponent<DrawableComponent>(mFrames[0], sf::Vector2f(0.375, 0.375));

	projectile.AddComponent<DamageComponent>(15);
}
	
void Book::LoadTextures()
{
	mFrames = TextureManager::GetTextures("assets/weapon/Book.png", 95, 128);
}