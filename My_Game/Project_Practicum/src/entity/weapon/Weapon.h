#pragma once

class EntityManager;
struct TransformComponent;

class Weapon
{
public:
	virtual ~Weapon() = default;
	virtual void Attack(EntityManager& entityManager, TransformComponent* parentTransform) = 0;
	float projectileSpeed = 0.0f;
	float fireRate = 0.0f;
	float cooldown = 0.0f;
};