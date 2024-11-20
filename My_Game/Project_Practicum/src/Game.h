#pragma once

#include "SFML/Graphics.hpp"
#include "Manager/System/SystemManager.h"
#include "Manager/Entity/EntityManager.h"

class Game
{
public:
	Game(sf::RenderWindow& window, sf::View& camera) 
		: mWindow(window), mCamera(camera) {}

	void init()
	{
		InitSystems();
		InitPlayer();
	}

	void Render(float deltaTime);
	void RunFrame(float deltaTime);
	bool IsPlayerDefeated();

	void ProcessEvents();
		
private:
	void InitSystems();
	void InitPlayer();

	sf::View& mCamera;
	sf::RenderWindow& mWindow;
	SystemManager mSystemManager;
	EntityManager mEntityManager;
};