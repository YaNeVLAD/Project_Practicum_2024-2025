#pragma once

#include "../System.h"

class CameraSystem : public IRenderSystem
{
public:
	CameraSystem(sf::View& camera) : mView(camera) {}

	void Render(EntityManager& entityManager, float deltaTime) override;

private:
	sf::View& mView;
};