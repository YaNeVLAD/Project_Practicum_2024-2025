#pragma once

#include "SFML/Graphics.hpp"
#include "SystemManager.h"
#include "EntityManager.h"

class Game
{
public:
	Game();

	void init()
	{
		initSystems();
		initPlayer();
		initEnemy();
	}

	void Run();

private:
	void initSystems();
	void initPlayer();
	void initEnemy();
	void ProcessEvents();

	sf::RenderWindow mWindow;
	SystemManager mSystemManager;
	EntityManager mEntityManager;
};