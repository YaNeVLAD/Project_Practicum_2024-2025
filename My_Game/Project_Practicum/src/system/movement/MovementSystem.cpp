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

		auto container = entity->GetComponent<ContainerComponent>();

		auto animation = entity->GetComponent<AnimationComponent>();

		auto parabolic = entity->GetComponent<ParaboloidProjectileComponent>();

		if (entity->GetType() & EntityType::Enemy && container == nullptr)
		{
			MoveTowardsTarget(*transform, playerPosition, 50.0f, deltaTime);
		}

		if (collision != nullptr)
		{
			collision->UpdatePosition(transform->x, transform->y);
		}

		if (transform->vx != 0 || transform->vy != 0)
		{
			UpdateDirectionAndRotation(transform, rotation, collision);
		}

		if (parabolic != nullptr)
		{
			transform->vy += parabolic->gravity * deltaTime;
		}

		transform->x += (transform->vx * transform->multiplier.x) * deltaTime;
		transform->y += (transform->vy * transform->multiplier.y) * deltaTime;

		if (animation != nullptr)
		{
			(transform->vx * transform->multiplier.x == 0 && transform->vy * transform->multiplier.y == 0)
				? animation->SetState(AnimationComponent::IDLE)
				: animation->SetState(AnimationComponent::WALK);
		}
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

void MovementSystem::UpdateDirectionAndRotation(TransformComponent* transform, RotationComponent* rotation, CollisionComponent* collision)
{
	sf::Vector2f newDirection = sf::Vector2f(
		std::copysign(1.0f, transform->vx),
		std::copysign(1.0f, transform->vy)
	);

	if (transform->lastDirection.x == newDirection.x)
	{
		return;
	}
	transform->lastDirection = newDirection;

	if (rotation == nullptr)
	{
		return;
	}
	float angle = std::atan2(transform->lastDirection.y, transform->lastDirection.x) * (180.0f / 3.14159f);
	rotation->angle = angle;

	if (collision == nullptr)
	{
		return;
	}
	collision->shape->setRotation(angle);
}