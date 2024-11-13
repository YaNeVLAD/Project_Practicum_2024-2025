#pragma once

#include "SFML/Graphics.hpp"
#include "System.h"

class CameraSystem : public System
{
public:
	CameraSystem(sf::RenderWindow& window) : mWindow(window)
	{
		mView = window.getDefaultView();
	}

	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	sf::RenderWindow& mWindow;
	sf::View mView;
};