#pragma once

#include "../System.h"

class InterfaceSystem : public IRenderSystem
{
public:
	InterfaceSystem(sf::RenderWindow& window, sf::View& camera) 
		: mWindow(window), mCamera(camera) {}

	void Render(EntityManager& entityManager, float deltaTime) override;

private:
	sf::RenderWindow& mWindow;
	sf::View& mCamera;

	void RenderHealth(EntityManager& em);
	void RenderXPBar(EntityManager& em);
};

