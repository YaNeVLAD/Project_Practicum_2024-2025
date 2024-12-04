#include "LightningStrike.h"

#include "../../../Manager/Texture/TextureManager.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"
#include <limits>

void LightningStrike::Upgrade(int level)
{
	if (mLevel + 1 <= MAX_LEVELS)
	{
		mLevel++;
		fireRate += 1.0f;
		damage += 15;
		mScale += {0.25f, 0.25f};
	}
}

void LightningStrike::Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform)
{
	auto enemies = entityManager.GetEntitiesWithType(Enemy);
	
	Entity* target = FindFarthestEnemy(enemies, playerTransform);

	if (target == nullptr)
	{
		return;
	}

	auto transform = target->GetComponent<TransformComponent>();

	if (transform == nullptr)
	{
		return;
	}

	Entity& lightning = entityManager.CreateEntity(EntityType::Projectile);
	lightning.AddComponent<DrawableComponent>(mFrames[2], mScale);
	lightning.AddComponent<LifetimeComponent>(0.3f);
	lightning.AddComponent<TransformComponent>(transform->x, transform->y, 0.0f, 0.0f);

	lightning.AddComponent<AnimationComponent>(mFrames, 0.15f, false, 0.3f, true);

	auto collisionShape = std::make_unique<sf::RectangleShape>(
		sf::Vector2f(
			mCollisionSize.x * mScale.x,
			mCollisionSize.y * mScale.y)
	);
	collisionShape->setOrigin(32.0f, 32.0f);
	lightning.AddComponent<CollisionComponent>(std::move(collisionShape));

	lightning.AddComponent<DamageComponent>(damage, Enemy);
}

Entity* LightningStrike::FindFarthestEnemy(std::vector<Entity*>& enemies, TransformComponent* transform) const
{
	if (enemies.empty())
	{
		return nullptr;
	}

	Entity* farthestEnemy = nullptr;
	float maxDistanceSquared = 0.0f;

	for (auto& enemy : enemies)
	{
		auto enemyTransform = enemy->GetComponent<TransformComponent>();
		if (enemyTransform == nullptr)
		{
			continue;
		}

		float dx = transform->x - enemyTransform->x;
		float dy = transform->y - enemyTransform->y;
		float distanceSquared = dx * dx + dy * dy;

		if (distanceSquared <= mMaxDistance * mMaxDistance)
		{
			if (distanceSquared > maxDistanceSquared)
			{
				maxDistanceSquared = distanceSquared;
				farthestEnemy = enemy;
			}
		}
	}

	return farthestEnemy;
}

Entity* LightningStrike::FindNextChainTarget(std::vector<Entity*>& enemies, Entity* currentTarget) const
{
	if (currentTarget == nullptr)
	{
		return nullptr;
	}

	auto transform = currentTarget->GetComponent<TransformComponent>();

	if (transform == nullptr)
	{
		return nullptr;
	}

	Entity* nextTarget = nullptr;
	float minDistance = mChainRadius * mChainRadius;

	for (auto& enemy : enemies)
	{
		if (enemy == currentTarget)
		{
			continue;
		}

		auto enemyTransform = enemy->GetComponent<TransformComponent>();

		if (enemyTransform == nullptr)
		{
			return nullptr;
		}

		float dx = transform->x - enemyTransform->x;
		float dy = transform->y - enemyTransform->y;
		float distanceSquared = dx * dx + dy * dy;

		if (distanceSquared < minDistance)
		{
			minDistance = distanceSquared;
			nextTarget = enemy;
		}
	}

	return nextTarget;
}

void LightningStrike::LoadTextures()
{
	mFrames = TextureManager::GetTextures("assets/weapon/Charge.png", 64, 64);
}