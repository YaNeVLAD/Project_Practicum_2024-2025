#include "Book.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"
#include "../../../../utils/SpriteSheet.h"

void Book::Attack(EntityManager& entityManager, TransformComponent* parentTransform)
{
	Entity& projectile = entityManager.CreateEntity();

	projectile.AddComponent<DrawableComponent>(24, 48, sf::Color::Yellow);
	projectile.AddComponent<TransformComponent>(
		parentTransform->x, parentTransform->y, 0.0f, 0.0f
	);
	projectile.AddComponent<OrbitalProjectileComponent>(100.0f, 2.0f, parentTransform);
	projectile.AddComponent<LifetimeComponent>(4.0f);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(24, 48));
	collisionShape->setOrigin(12, 24);
	projectile.AddComponent<CollisionComponent>(std::move(collisionShape));
}

void Book::LoadTextures()
{
}