#pragma once

#include "../ui/Map/Map.h"
#include "SFML/Graphics.hpp"
#include "Manager/System/SystemManager.h"
#include "Manager/Entity/EntityManager.h"


class Game
{
public:
	Game(sf::RenderWindow& window, sf::View& camera) 
		: mWindow(window), mCamera(camera) {}

	void Reset();

	void Init()
	{
		InitKeyBindings();
		InitSystems();
		InitPlayer();
		InitMap();
	}

	void Restart()
	{
		Reset();
		Init();
	}

	void Render(float deltaTime);
	void RunFrame(float deltaTime);

	void Pause();
	void Resume();
	
	bool IsBossDefeated();
	bool IsPlayerDefeated();
	bool HasPlayerLeveledUp();

	std::vector<std::string> GetAvailableWeapons();
	void UpgradeWeapon(std::string name);

	void ProcessEvents(const sf::Event& event);
		
private:
	bool mIsPaused = false;
	bool mIsBossSpawned = false;

	Map mMap;

	Screen mScreen;

	sf::View& mCamera;
	sf::RenderWindow& mWindow;
	SystemManager mSystemManager;
	EntityManager mEntityManager;

	bool CanPause();

	void InitKeyBindings();
	void InitSystems();
	void InitPlayer();
	void InitMap();

	void RenderPauseScreen();
};