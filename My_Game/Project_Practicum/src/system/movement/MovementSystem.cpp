#include "MovementSystem.h"

void MovementSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		auto transform = entity.GetComponent<TransformComponent>();

		if (transform)
		{
			if (transform->vx != 0 || transform->vy != 0)
			{
				float speedVectorLen = std::sqrt(transform->vx * transform->vx + transform->vy * transform->vy);
				transform->lastDirection = { transform->vx / speedVectorLen, transform->vy / speedVectorLen };
			}

			transform->x += transform->vx * deltaTime;
			transform->y += transform->vy * deltaTime;
		}
	}
}