#include "LifetimeSystem.h"

void LifetimeSystem::Update(EntityManager& entityManager, float deltaTime)
{
	std::vector<Entity*> entitiesToDelete;

	auto player = entityManager.GetEntitiesWithComponents<PlayerHealthComponent, ExperienceComponent>();

	for (auto& entity : entityManager.GetEntities())
	{
		auto lifetime = entity.GetComponent<LifetimeComponent>();
		auto health = entity.GetComponent<HealthComponent>();
		//auto transform = entity->GetComponent<TransformComponent>();

		if (health)
		{
			health->UpdateCooldown(deltaTime);

			if (!health->IsAlive())
			{
				if (entity.GetType() & Enemy && player.size())
				{
					auto experience = player.front()->GetComponent<ExperienceComponent>();
					if (experience)
					{
						experience->GainExperience(50);
					}
				}
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

		//if (IsOutOfBound(transform->x, transform->y))
		//{
		//    entitiesToDelete.push_back(entity);
		//}
	}

	for (auto& entity : entitiesToDelete)
	{
		entityManager.RemoveEntity(entity->GetId());
	}
}

//bool LifetimeSystem::IsOutOfBound(float x, float y)
//{
//    float left = mCamera.getCenter().x - mCamera.getSize().x / 2;
//    float top = mCamera.getCenter().y - mCamera.getSize().y / 2;
//    float right = left + mCamera.getSize().x;
//    float bottom = top + mCamera.getSize().y;
//
//    return x < left || x > right || y < top || y > bottom;
//}
