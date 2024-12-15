#pragma once

#include "../Weapon.h"

class Axe : public Weapon
{
public:
	Axe()
	{
		projectileSpeed = 300.f;
		fireRate = 1.5f;
		cooldown = 0.f;
		damage = 10;
		mGravity = projectileSpeed / 3;
	}

	void Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform) override;
	void Upgrade(int level) override;
	std::string GetName() override;
	int GetLevel() override;
	bool CanUpgrade() override;

private:
	const std::string WEAPON_NAME = "Axe";
	const int MAX_LEVELS = 4;
	int mLevel = 1;

	float mGravity = 9.8f;
};