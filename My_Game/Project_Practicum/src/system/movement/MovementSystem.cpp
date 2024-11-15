#include "MovementSystem.h"

void MovementSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<TransformComponent>())
	{
		auto transform = entity->GetComponent<TransformComponent>();
		auto rotation = entity->GetComponent<RotationComponent>();

		auto collision = entity->GetComponent<CollisionComponent>();

		if (collision)
		{
			collision->UpdatePosition(transform->x, transform->y);
		}

		if (transform->vx != 0 || transform->vy != 0)
		{
			float speedVectorLen = std::sqrt(transform->vx * transform->vx + transform->vy * transform->vy);
			transform->lastDirection = sf::Vector2f(transform->vx / speedVectorLen, transform->vy / speedVectorLen);

			if (rotation)
			{
				float angle = std::atan2(transform->lastDirection.y, transform->lastDirection.x) * (180.0f / 3.14159f);
				rotation->angle = angle;

				if (collision)
				{
					collision->shape->setRotation(angle);
				}
			}
		}

		transform->x += transform->vx * deltaTime;
		transform->y += transform->vy * deltaTime;
	}
}