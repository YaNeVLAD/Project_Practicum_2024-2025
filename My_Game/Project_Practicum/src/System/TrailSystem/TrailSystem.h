#pragma once

#include "../System.h"

class TrailSystem : public IUpdateSystem
{
public:
	TrailSystem()
	{
		LoadTextures();
	}

	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	std::vector<sf::Texture> mFrames;

	void LoadTextures();
	void CreateTrailProjectile(EntityManager& em, Entity* entity);
};