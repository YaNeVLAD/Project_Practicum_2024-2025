#pragma once

#include "../System.h"

class ProjectileSystem : public System
{
public:
	ProjectileSystem(sf::View& camera) : mCamera(camera) {}
	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	sf::View& mCamera;
	bool IsOutOfBound(float x, float y);
};