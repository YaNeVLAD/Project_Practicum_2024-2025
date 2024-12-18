#pragma once

#include "../Weapon.h"
#include <vector>
#include "SFML/Graphics.hpp"

class Book : public Weapon
{
public:
	Book()
	{
		fireRate = 5.0f;
		damage = 10;
		projectileSpeed = 125.0f;
		LoadTextures();
	}

	std::string GetName() { return WEAPON_NAME; }
	void Upgrade(int level) override;
	void Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform) override;
	bool CanUpgrade() override { return mLevel < MAX_LEVELS; }
	int GetLevel() override { return mLevel; }

private:
	const std::string WEAPON_NAME = "Holy Book";
	const int MAX_LEVELS = 4;
	int mLevel = 0;

	float mLifetime = 3.0f;

	void LoadTextures();
	std::vector<sf::Texture> mFrames;
};