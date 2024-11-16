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

	void Attack(EntityManager& entityManager, TransformComponent* parentTransform) override;

private:
	void LoadTextures();
	std::vector<sf::Texture> mFrames;
};