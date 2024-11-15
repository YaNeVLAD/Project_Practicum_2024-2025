#include "MovementSystem.h"

void MovementSystem::Update(EntityManager& entityManager, float deltaTime)
{
	auto players = entityManager.GetEntitiesWithComponents<PlayerHealthComponent, TransformComponent>();
	if (players.empty()) return;

	auto* player = players.front();
	auto playerTransform = player->GetComponent<TransformComponent>();
	sf::Vector2f playerPosition(playerTransform->x, playerTransform->y);

	for (auto& entity : entityManager.GetEntitiesWithComponents<TransformComponent>())
	{
		auto transform = entity->GetComponent<TransformComponent>();
		auto rotation = entity->GetComponent<RotationComponent>();

		auto collision = entity->GetComponent<CollisionComponent>();

		if (entity->GetType() & EntityType::Enemy)
		{
			MoveTowardsTarget(*transform, playerPosition, 100.0f, deltaTime); // Скорость врага 100
		}

		if (collision)
		{
			collision->UpdatePosition(transform->x, transform->y);
		}

		if (transform->vx != 0 || transform->vy != 0)
		{
			UpdateDirectionAndRotation(*transform, rotation, collision);
		}

		transform->x += transform->vx * deltaTime;
		transform->y += transform->vy * deltaTime;
	}
}

void MovementSystem::MoveTowardsTarget(TransformComponent& transform, const sf::Vector2f targetPosition, float speed, float deltaTime)
{
	sf::Vector2f position(transform.x, transform.y);
	sf::Vector2f direction = targetPosition - position;

	float directionVectorLen = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (directionVectorLen > 0.0f)
	{
		direction /= directionVectorLen;
		transform.vx = direction.x * speed;
		transform.vy = direction.y * speed;
	}
}

void MovementSystem::UpdateDirectionAndRotation(TransformComponent& transform, RotationComponent* rotation, CollisionComponent* collision)
{
	float speedVectorLen = std::sqrt(transform.vx * transform.vx + transform.vy * transform.vy);
	transform.lastDirection = sf::Vector2f(transform.vx / speedVectorLen, transform.vy / speedVectorLen);

	if (rotation)
	{
		float angle = std::atan2(transform.lastDirection.y, transform.lastDirection.x) * (180.0f / 3.14159f);
		rotation->angle = angle;

		if (collision)
		{
			collision->shape->setRotation(angle);
		}
	}
}
