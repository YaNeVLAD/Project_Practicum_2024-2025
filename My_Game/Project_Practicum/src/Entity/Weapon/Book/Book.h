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
		projectileSpeed = 150.0f;
		LoadTextures();
	}
	void Attack(EntityManager& entityManager, TransformComponent* parentTransform) override;

private:
	void LoadTextures();
	std::vector<sf::Texture> mFrames;
};