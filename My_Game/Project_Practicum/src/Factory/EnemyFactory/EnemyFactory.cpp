#include "EnemyFactory.h"

#include "../../Manager/Entity/EntityManager.h"

void EnemyFactory::Create(EntityManager& entityManager, float x, float y)
{
	auto& enemy = entityManager.CreateEntity(EntityType::Enemy);
	enemy.AddComponent<TransformComponent>(x, y, 0.0f, 0.0f);
	enemy.AddComponent<DrawableComponent>(40, 40, sf::Color::Red);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 40));
	collisionShape->setOrigin(20.0f, 20.0f);
	enemy.AddComponent<CollisionComponent>(std::move(collisionShape));
	enemy.AddComponent<HealthComponent>(50, 0.5f);
}
