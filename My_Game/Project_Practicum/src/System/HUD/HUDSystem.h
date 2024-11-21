#pragma once

#include "../System.h"

class HUDSystem : public IRenderSystem
{
public:
	HUDSystem(sf::RenderWindow& window, sf::View& camera) 
		: mWindow(window), mCamera(camera) {}

	void Render(EntityManager& entityManager, float deltaTime) override;

private:
	sf::RenderWindow& mWindow;
	sf::View& mCamera;

	void RenderHealth(EntityManager& em);
	void RenderPlayerHealth(EntityManager& em);
	void RenderXPBar(EntityManager& em);
};

