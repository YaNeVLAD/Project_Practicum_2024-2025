#pragma once

#include "../Weapon.h"
#include "SFML/Graphics.hpp"

class Fireball : public Weapon
{
public:
	Fireball()
	{
		fireRate = 2.0f;
		projectileSpeed = 250.0f;
		damage = 5;
		LoadTextures();
	}

	std::string GetName() { return WEAPON_NAME; }
	void Upgrade(int level) override;
	void Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform) override;
	bool CanUpgrade() override { return mLevel < MAX_LEVELS; }
	int GetLevel() override { return mLevel; }

private:
	const std::string WEAPON_NAME = "Fireball";

	const int MAX_LEVELS = 4;
	int mLevel = 0;

	int mTrailDamage = 5;
	sf::Vector2f mScale = { 1,1 };

	std::vector<sf::Texture> mFrames;

	void LoadTextures();
};