#include "HomingProjectileSystem.h"

#include "../../Manager/Entity/EntityManager.h"

void HomingProjectileSystem::Update(EntityManager& entityManager, float deltaTime)
{
	auto projectiles = entityManager.GetEntitiesWithComponents<HomingProjectileComponent, TransformComponent, CollisionComponent>();
	auto enemies = entityManager.GetEntitiesWithComponents<TransformComponent, HealthComponent>();

	for (auto& projectileEntity : projectiles)
	{
		auto projectileTransform = projectileEntity->GetComponent<TransformComponent>();
		auto homingComponent = projectileEntity->GetComponent<HomingProjectileComponent>();
		auto collision = projectileEntity->GetComponent<CollisionComponent>();

		auto rotation = projectileEntity->GetComponent<RotationComponent>();

		Entity* closestEnemy = FindClosestTarget(projectileTransform, enemies);
		if (!closestEnemy)
		{
			continue;
		}

		auto enemyTransform = closestEnemy->GetComponent<TransformComponent>();
		sf::Vector2f direction(enemyTransform->x - projectileTransform->x, enemyTransform->y - projectileTransform->y);

		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length > 0.0f)
		{
			direction /= length;
		}

		projectileTransform->x += direction.x * homingComponent->speed * deltaTime;
		projectileTransform->y += direction.y * homingComponent->speed * deltaTime;

		if (rotation)
		{
			rotation->angle = std::atan2(direction.y, direction.x) * (180.0f / 3.14159265f);
			collision->shape->setRotation(rotation->angle);
		}
	}
}

Entity* HomingProjectileSystem::FindClosestTarget(TransformComponent* projectile, const std::vector<Entity*>& enemies)
{
	Entity* closestEnemy = nullptr;
	float minDistanceSquared = std::numeric_limits<float>::max();

	for (auto& enemy : enemies)
	{
		auto enemyTransform = enemy->GetComponent<TransformComponent>();
		float dx = projectile->x - enemyTransform->x;
		float dy = projectile->y - enemyTransform->y;
		float distanceSquared = dx * dx + dy * dy; \

			if (distanceSquared < minDistanceSquared)
			{
				minDistanceSquared = distanceSquared;
				closestEnemy = enemy;
			}
	}

	return closestEnemy;
}