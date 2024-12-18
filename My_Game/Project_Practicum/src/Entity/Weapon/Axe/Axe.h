#pragma once

#include "../Weapon.h"
#include "SFML/Graphics.hpp"

class Axe : public Weapon
{
public:
	Axe()
	{
		projectileSpeed = 800.f;
		fireRate = 1.5f;
		cooldown = fireRate;
		damage = 8;
		mGravity = projectileSpeed;
		LoadTextures();
	}

	void Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform) override;
	void Upgrade(int level) override;
	std::string GetName() override;
	int GetLevel() override;
	bool CanUpgrade() override;

private:
	const std::string WEAPON_NAME = "Axe";
	const int MAX_LEVELS = 4;
	int mLevel = 0;

	float mGravity = 9.8f;
	
	std::vector<sf::Texture> mFrames;

	void LoadTextures();
};