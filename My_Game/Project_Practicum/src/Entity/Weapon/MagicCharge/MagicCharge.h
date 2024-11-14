#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "../Weapon.h"

class MagicCharge : public Weapon
{
public:
	MagicCharge() 
	{
		fireRate = 1.0f; 
		projectileSpeed = 150.0f; 
		LoadTextures(); 
	}

	void Attack(Entity& projectile, const TransformComponent& startPosition) override;

private:
	void LoadTextures();
	std::vector<sf::Texture> mFrames;
};