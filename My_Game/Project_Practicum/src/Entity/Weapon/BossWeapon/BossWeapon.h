#pragma once

#include "SFML/Graphics.hpp"
#include "../Weapon.h"

class BossWeapon : public Weapon
{
public:
	BossWeapon()
	{
		fireRate = 1.25f;
		projectileSpeed = 300.0f;
		LoadTextures();
	}

	std::string GetName() { return WEAPON_NAME; }
	void Upgrade(int level) override { return; }
	void Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform) override;
	bool CanUpgrade() override { return false; }
	int GetLevel() override { return mLevel; }

private:
	const std::string WEAPON_NAME = "Boss Weapon";
	int mLevel = -1;

	void LoadTextures();
	std::vector<sf::Texture> mFrames;
};