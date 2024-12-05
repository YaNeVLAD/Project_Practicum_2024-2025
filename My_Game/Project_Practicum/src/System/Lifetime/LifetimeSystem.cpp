#include "LifetimeSystem.h"
#include "../../Factory/Factory.h"
#include <iostream>

void LifetimeSystem::Update(EntityManager& entityManager, float deltaTime)
{
	std::vector<Entity::IdType> entitiesToDelete;

	auto player = entityManager.GetEntitiesWithComponents<PlayerHealthComponent>();
	auto playerHealth = player.front()->GetComponent<PlayerHealthComponent>();

	for (auto& entity : entityManager.GetEntities())
	{
		auto health = entity.GetComponent<HealthComponent>();
		auto lifetime = entity.GetComponent<LifetimeComponent>();
		auto bossHealth = entity.GetComponent<BossHealthComponent>();

		auto transform = entity.GetComponent<TransformComponent>();

		if (bossHealth != nullptr)
		{
			if (!bossHealth->IsAlive())
			{
				entitiesToDelete.push_back(entity.GetId());
			}
		}

		if (health != nullptr)
		{
			if (!health->IsAlive())
			{
				if (transform != nullptr)
				{
					Factory::CreateExperience(entityManager, sf::Vector2f(transform->x, transform->y));
				}

				entitiesToDelete.push_back(entity.GetId());
			}
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