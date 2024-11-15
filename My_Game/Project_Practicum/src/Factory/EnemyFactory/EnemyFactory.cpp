#include "EnemyFactory.h"

#include "../../Manager/Entity/EntityManager.h"
#include "../../../utils/SpriteSheet.h"

void EnemyFactory::Create(EntityManager& entityManager, float x, float y)
{
	auto& enemy = entityManager.CreateEntity(EntityType::Enemy);
	enemy.AddComponent<TransformComponent>(x, y, 0.0f, 0.0f);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(50, 50));
	collisionShape->setOrigin(25, 25);
	enemy.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 15));
	enemy.AddComponent<HealthComponent>(50, 0.5f);

	std::vector<sf::Texture> frames = SpriteSheet::LoadTextures("assets/melee_enemy/Walk.png", 128, 128);
	enemy.AddComponent<AnimationComponent>(frames, 0.2f, true, -1.0f, true);
	enemy.AddComponent<DrawableComponent>(frames[0], sf::Vector2f(0.75f, 0.75f));
}