#pragma once

class Entity;
struct TransformComponent;

class Weapon
{
public:
	virtual ~Weapon() = default;
	virtual void Attack(Entity& projectile, const TransformComponent & transform) = 0;
	float fireRate = 0.0f;
	float cooldown = 0.0f;
};