#pragma once

#include "../System.h"

class CameraSystem : public System
{
public:
	CameraSystem(sf::View& camera) : mView(camera) {}

	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	sf::View& mView;
};