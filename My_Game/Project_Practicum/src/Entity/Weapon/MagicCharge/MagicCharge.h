#pragma once

#include "../Weapon.h"
#include <vector>
#include "SFML/Graphics.hpp"

class MagicCharge : public Weapon
{
public:
	MagicCharge() 
	{
		fireRate = 1.0f;
		projectileSpeed = 250.0f; 
		LoadTextures(); 
	}

	std::string GetName() { return WEAPON_NAME; }
	void Upgrade(int level) override;
	void Attack(EntityManager& entityManager, TransformComponent* parentTransform) override;
	bool CanUpgrade() override { return mLevel < MAX_LEVELS; }
	int GetLevel() override { return mLevel; }

private:
	const std::string WEAPON_NAME = "Magic Charge";
	const int MAX_LEVELS = 4;
	int mLevel = 1;

	void LoadTextures();
	std::vector<sf::Texture> mFrames;
};