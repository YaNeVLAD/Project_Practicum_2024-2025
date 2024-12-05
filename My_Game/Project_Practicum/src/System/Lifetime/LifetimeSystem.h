#pragma once

#include "../System.h"

class LifetimeSystem : public IUpdateSystem
{
public:
	LifetimeSystem(sf::View& camera) : mCamera(camera) {}
	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	sf::View& mCamera;
};