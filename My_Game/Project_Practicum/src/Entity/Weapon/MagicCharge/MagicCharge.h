#pragma once

#include "../Weapon.h"
#include <vector>
#include "SFML/Graphics.hpp"

class MagicCharge : public Weapon
{
public:
	MagicCharge() 
	{
		fireRate = 1.f;
		projectileSpeed = 250.f; 
		damage = 5;
		LoadTextures(); 
	}

	std::string GetName() { return WEAPON_NAME; }
	void Upgrade(int level) override;
	void Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform) override;
	bool CanUpgrade() override { return mLevel < MAX_LEVELS; }
	int GetLevel() override { return mLevel; }

private:
	const std::string WEAPON_NAME = "Magic Charge";
	const int MAX_LEVELS = 3;
	int mLevel = 0;

	void LoadTextures();
	std::vector<sf::Texture> mFrames;
};