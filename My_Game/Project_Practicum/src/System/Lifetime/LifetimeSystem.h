#pragma once

#include "../System.h"

class LifetimeSystem : public System
{
public:
	LifetimeSystem(sf::View& camera) : mCamera(camera) {}
	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	sf::View& mCamera;
	//bool IsOutOfBound(float x, float y);
};