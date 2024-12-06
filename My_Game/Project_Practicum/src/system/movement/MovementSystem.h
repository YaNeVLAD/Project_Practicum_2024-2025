#pragma once

#include "../System.h"

class MovementSystem : public IUpdateSystem
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	void MoveTowardsTarget(TransformComponent& transform, const sf::Vector2f targetPosition, float speed, float deltaTime);
	void UpdateDirectionAndRotation(TransformComponent* transform, RotationComponent* rotation, CollisionComponent* collision);
};