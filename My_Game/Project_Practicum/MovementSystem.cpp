#include "MovementSystem.h"

void MovementSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		auto transform = entity.GetComponent<TransformComponent>();

		if (transform)
		{
			transform->x += transform->vx * deltaTime;
			transform->y += transform->vy * deltaTime;
		}
	}
}