#pragma once

#include "../System.h"

class InterfaceSystem : public IRenderSystem
{
public:
	InterfaceSystem(sf::RenderWindow& windonw) : mWindow(windonw) {}

	void Render(EntityManager& entityManager, float deltaTime) override;

private:
	sf::RenderWindow& mWindow;
};

