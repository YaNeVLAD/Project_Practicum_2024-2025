#pragma once

#include "../Weapon.h"
#include "SFML/Graphics.hpp"

class Entity;

class LightningStrike : public Weapon
{
public:
	LightningStrike()
	{
		fireRate = 1.0f;
		damage = 15;
		LoadTextures();
	}

	std::string GetName() { return WEAPON_NAME; }
	void Upgrade(int level) override;
	void Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform) override;
	bool CanUpgrade() override { return mLevel < MAX_LEVELS; }
	int GetLevel() override { return mLevel; }

private:
	const std::string WEAPON_NAME = "Lightning Strike";

	const int MAX_LEVELS = 4;
	int mLevel = 1;

	int mChainCount = 1;
	float mChainRadius = 50.0f;
	float mMaxDistance = 400.0f;
	
	sf::Vector2f mCollisionSize = { 32, 32 };
	sf::Vector2f mScale = { 1.25f, 1.25f };

	std::vector<sf::Texture> mFrames;

	Entity* FindFarthestEnemy(std::vector<Entity*>&, TransformComponent* transform) const;

	void LoadTextures();
};