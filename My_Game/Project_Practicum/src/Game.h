#pragma once

#include "SFML/Graphics.hpp"
#include "Manager/System/SystemManager.h"
#include "Manager/Entity/EntityManager.h"

class Game
{
public:
	Game();

	void init()
	{
		InitSystems();
		InitPlayer();
	}

	void Run();

private:
	void InitSystems();
	void InitPlayer();
	void ProcessEvents();

	sf::View mCamera;
	sf::RenderWindow mWindow;
	SystemManager mSystemManager;
	EntityManager mEntityManager;
};