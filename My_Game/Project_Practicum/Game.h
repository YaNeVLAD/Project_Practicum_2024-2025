#pragma once

#include "SFML/Graphics.hpp"
#include "SystemManager.h"
#include "EntityManager.h"

class Game
{
public:
	Game();

	void Run();

private:
	void ProcessEvents();

	sf::RenderWindow mWindow;
	SystemManager mSystemManager;
	EntityManager mEntityManager;
	std::vector<std::unique_ptr<System>> mSystems;
};