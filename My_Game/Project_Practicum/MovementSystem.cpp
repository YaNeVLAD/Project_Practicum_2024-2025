#include "MovementSystem.h"

void MovementSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		auto position = entity.GetComponent<PositionComponent>();
		auto velocity = entity.GetComponent<VelocityComponent>();

		if (position && velocity)
		{
			position->x += velocity->vx * deltaTime;
			position->y += velocity->vy * deltaTime;
		}
	}
}