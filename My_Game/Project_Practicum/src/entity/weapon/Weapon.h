#pragma once

#include <string>
#include <vector>

class EntityManager;
struct TransformComponent;

class Weapon
{
public:
	float projectileSpeed = 0.0f;
	float fireRate = 0.0f;
	float cooldown = 0.0f;
	int damage = 0;

	virtual ~Weapon() = default;
	virtual void Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform) = 0;
	virtual void Upgrade(int level) = 0;
	virtual std::string GetName() = 0;
	virtual int GetLevel() = 0;
	virtual bool CanUpgrade() = 0;

	static std::vector<std::string> GetAllWeapons()
	{
		return { "Magic Charge", "Holy Book", "Fireball", "Lightning Strike"};
	}
};