#include "LifetimeSystem.h"

#include "../../Factory/Factory.h"
#include <iostream>

void LifetimeSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& player : entityManager.GetEntitiesWithType(Player))
	{
		if (auto health = player->GetComponent<PlayerHealthComponent>())
		{
			TryApplyDeathAnimation(player, health);
		}
	}

	for (auto& boss : entityManager.GetEntitiesWithComponents<BossHealthComponent>())
	{
		if (auto health = boss->GetComponent<BossHealthComponent>())
		{
			TryApplyDeathAnimation(boss, health);
		}
	}

	std::vector<Entity::IdType> entitiesToDelete;

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
	}

	for (const auto& id : entitiesToDelete)
	{
		entityManager.RemoveEntity(id);
	}
}

void LifetimeSystem::TryApplyDeathAnimation(Entity* entity, auto* health)
{
	auto animation = entity->GetComponent<AnimationComponent>();
	auto deathAnimation = entity->GetComponent<DeathAnimationComponent>();

	if (health != nullptr && !health->IsAlive() && deathAnimation == nullptr)
	{
		animation->SetState(AnimationComponent::DEAD);
		animation->frameTime = 0.45f;
		animation->loop = false;
		entity->AddComponent<DeathAnimationComponent>();
	}
}