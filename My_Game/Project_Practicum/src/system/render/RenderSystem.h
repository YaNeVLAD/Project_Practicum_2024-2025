#pragma once

#include "../System.h"

class RenderSystem : public System
{
public:
	RenderSystem(sf::RenderWindow& window) : mWindow(window) {}

	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	sf::RenderWindow& mWindow;
};