#include "BossWeapon.h"

#include "../../../Manager/Texture/TextureManager.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"

void BossWeapon::Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform)
{
	sf::Vector2f direction(playerTransform->x - parentTransform->x, playerTransform->y - parentTransform->y);
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (length != 0.0f)
	{
		direction.x /= length;
		direction.y /= length;
	}

	Entity& projectile = entityManager.CreateEntity(EntityType::Projectile);
	projectile.AddComponent<TransformComponent>(
		parentTransform->GetPosition(), direction * projectileSpeed
	);
	projectile.AddComponent<LifetimeComponent>(1.5f);
	projectile.AddComponent<DrawableComponent>(mFrames[0], sf::Vector2f(1.5, 1.5));
	projectile.AddComponent<AnimationComponent>(mFrames, 0.2f, true, -1.0f, true);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	projectile.AddComponent<CollisionComponent>(std::move(collisionShape));

	projectile.AddComponent<DamageComponent>(10, 0.5f, Player);
	projectile.AddComponent<RotationComponent>();
}

void BossWeapon::LoadTextures()
{
	mFrames = TextureManager::GetTextures("assets/weapon/Fireball.png", 64, 64);
}
