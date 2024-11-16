#include "Book.h"

#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"
#include "../../../../utils/SpriteSheet.h"

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
}
	
void Book::LoadTextures()
{
	mFrames = SpriteSheet::LoadTextures("assets/weapon/Book.png", 95, 128);
}