#include "HomingProjectileSystem.h"

#include "../../Manager/Entity/EntityManager.h"

void HomingProjectileSystem::Update(EntityManager& entityManager, float deltaTime)
{
	auto projectiles = entityManager.GetEntitiesWithComponents<HomingProjectileComponent, TransformComponent, CollisionComponent>();

	for (auto& projectileEntity : projectiles)
	{
		auto projectileTransform = projectileEntity->GetComponent<TransformComponent>();
		auto homingComponent = projectileEntity->GetComponent<HomingProjectileComponent>();
		auto collision = projectileEntity->GetComponent<CollisionComponent>();

		auto rotation = projectileEntity->GetComponent<RotationComponent>();

		Entity* closestTarget = FindClosestTarget(entityManager, projectileTransform, homingComponent->targetType);
		if (closestTarget == nullptr)
		{
			continue;
		}

		auto enemyTransform = closestTarget->GetComponent<TransformComponent>();
		sf::Vector2f direction(enemyTransform->x - projectileTransform->x, enemyTransform->y - projectileTransform->y);

		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length > 0.0f)
		{
			direction /= length;
		}

		projectileTransform->x += direction.x * homingComponent->speed * deltaTime;
		projectileTransform->y += direction.y * homingComponent->speed * deltaTime;

		if (rotation != nullptr)
		{
			rotation->angle = std::atan2(direction.y, direction.x) * (180.0f / 3.14159265f);
			collision->shape->setRotation(rotation->angle);
		}
	}
}

Entity* HomingProjectileSystem::FindClosestTarget(EntityManager& em, TransformComponent* projectile, const EntityType& targetType)
{
	Entity* closestEnemy = nullptr;
	float minDistanceSquared = std::numeric_limits<float>::max();

	for (auto& enemy : em.GetEntitiesWithType(targetType))
	{
		auto enemyTransform = enemy->GetComponent<TransformComponent>();

		if (enemyTransform == nullptr)
		{
			return nullptr;
		}

		float dx = projectile->x - enemyTransform->x;
		float dy = projectile->y - enemyTransform->y;
		float distanceSquared = dx * dx + dy * dy;

		if (distanceSquared < minDistanceSquared)
		{
			minDistanceSquared = distanceSquared;
			closestEnemy = enemy;
		}
	}

	return closestEnemy;
}