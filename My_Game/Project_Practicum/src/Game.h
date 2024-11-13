#pragma once

#include "SFML/Graphics.hpp"
#include "manager/system/SystemManager.h"
#include "manager/entity/EntityManager.h"

class Game
{
public:
	Game();

	void init()
	{
		InitSystems();
		InitPlayer();
		InitEnemy();
	}

	void Run();

private:
	void InitSystems();
	void InitPlayer();
	void InitEnemy();
	void ProcessEvents();

	sf::View mCamera;
	sf::RenderWindow mWindow;
	SystemManager mSystemManager;
	EntityManager mEntityManager;
};