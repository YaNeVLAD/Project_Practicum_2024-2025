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

	std::string GetName() { return WEAPON_NAME; }
	void Upgrade(int level) override;
	void Attack(EntityManager& entityManager, TransformComponent* parentTransform) override;

private:
	const std::string WEAPON_NAME = "Holy Book";

	void LoadTextures();
	std::vector<sf::Texture> mFrames;
};