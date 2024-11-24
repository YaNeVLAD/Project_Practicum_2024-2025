#include "LifetimeSystem.h"

void LifetimeSystem::Update(EntityManager& entityManager, float deltaTime)
{
	std::vector<Entity*> entitiesToDelete;

	auto player = entityManager.GetEntitiesWithComponents<PlayerHealthComponent, ExperienceComponent>();
	auto playerHealth = player.front()->GetComponent<PlayerHealthComponent>();
	playerHealth->UpdateCooldown(deltaTime);

	for (auto& entity : entityManager.GetEntities())
	{
		auto lifetime = entity.GetComponent<LifetimeComponent>();
		auto health = entity.GetComponent<HealthComponent>();
		auto bossHealth = entity.GetComponent<BossHealthComponent>();

		if (bossHealth)
		{
			bossHealth->UpdateCooldown(deltaTime);
			if (!bossHealth->IsAlive())
			{
				GainPlayerExperience(entity, *player.front());

				entitiesToDelete.push_back(&entity);
			}
		}

		if (health)
		{
			health->UpdateCooldown(deltaTime);

			if (!health->IsAlive())
			{
				GainPlayerExperience(entity, *player.front());

				entitiesToDelete.push_back(&entity);
			}
		}

		if (lifetime)
		{
			lifetime->time -= deltaTime;

			if (lifetime->time <= 0.0f) {
				entitiesToDelete.push_back(&entity);
				continue;
			}
		}
	}

	for (auto& entity : entitiesToDelete)
	{
		entityManager.RemoveEntity(entity->GetId());
	}
}

void LifetimeSystem::GainPlayerExperience(Entity& enemy, Entity& player)
{
	if (enemy.GetType() & Enemy && player.GetType())
	{
		auto experience = player.GetComponent<ExperienceComponent>();
		auto playerHealth = player.GetComponent<PlayerHealthComponent>();

		if (experience && playerHealth)
		{
			experience->GainExperience(50);

			if (experience->CheckLevelUp())
			{
				playerHealth->LevelUp(10);
			}
		}
	}
}