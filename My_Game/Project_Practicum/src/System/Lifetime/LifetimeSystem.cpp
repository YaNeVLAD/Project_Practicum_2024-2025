#include "LifetimeSystem.h"

#include "../../Factory/Factory.h"
#include <iostream>

void LifetimeSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& player : entityManager.GetEntitiesWithComponents<PlayerHealthComponent>())
	{
		if (auto health = player->GetComponent<PlayerHealthComponent>())
		{
			TryApplyDeathAnimation(entityManager, player, health);
		}
	}

	for (auto& boss : entityManager.GetEntitiesWithComponents<BossHealthComponent>())
	{
		if (auto health = boss->GetComponent<BossHealthComponent>())
		{
			TryApplyDeathAnimation(entityManager, boss, health);
		}
	}

	std::vector<Entity::IdType> entitiesToDelete;

	sf::Vector2f cameraCenter = mCamera.getCenter();
	sf::Vector2f cameraSize = mCamera.getSize();

	float maxDistanceX = 1.5f * (cameraSize.x / 2.f);
	float maxDistanceY = 1.5f * (cameraSize.y / 2.f);

	for (auto& entity : entityManager.GetEntities())
	{
		auto health = entity.GetComponent<HealthComponent>();
		auto lifetime = entity.GetComponent<LifetimeComponent>();

		auto transform = entity.GetComponent<TransformComponent>();

		if (health != nullptr && !health->IsAlive())
		{
			if (transform != nullptr)
			{
				Factory::CreateExperience(entityManager, sf::Vector2f(transform->x, transform->y));
			}

			entitiesToDelete.push_back(entity.GetId());
		}

		if (lifetime != nullptr)
		{
			lifetime->time -= deltaTime;

			if (lifetime->time <= 0.0f)
			{
				entitiesToDelete.push_back(entity.GetId());
			}
		}

		if (transform != nullptr)
		{
			float dx = std::abs(transform->x - cameraCenter.x);
			float dy = std::abs(transform->y - cameraCenter.y);

			if (dx < maxDistanceX && dy < maxDistanceY)
			{
				continue;
			}

			if (!entity.HasComponent<PlayerHealthComponent>() && !entity.HasComponent<BossHealthComponent>() && entity.GetType() != EntityType::Particle)
			{
				entitiesToDelete.push_back(entity.GetId());
			}
		}
	}

	for (const auto& id : entitiesToDelete)
	{
		entityManager.RemoveEntity(id);
	}
}

void LifetimeSystem::TryApplyDeathAnimation(EntityManager& em, Entity* entity, auto* health)
{
	auto animation = entity->GetComponent<AnimationComponent>();
	auto deathAnimation = entity->GetComponent<DeathAnimationComponent>();
	auto bossDeath = entity->GetComponent<VictoryComponent>();
	auto playerDeath = entity->GetComponent<GameOverComponent>();

	if (playerDeath != nullptr || bossDeath != nullptr)
	{
		em.RemoveEntity(entity->GetId());
	}

	if (bossDeath != nullptr)
	{
		(*mDefeatedBosses)++;
	}
	if (health != nullptr && !health->IsAlive() && deathAnimation == nullptr)
	{
		animation->SetState(AnimationComponent::DEAD);
		animation->frameTime = 0.45f;
		animation->loop = false;
		entity->AddComponent<DeathAnimationComponent>();
	}
}