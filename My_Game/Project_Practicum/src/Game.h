#pragma once

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

	void PauseGame();
	void ResumeGame();

	bool IsPlayerDefeated();
	bool HasPlayerLeveledUp();
	bool IsBossDefeated();

	std::vector<std::string> GetAvailableWeapons();
	void UpgradeWeapon(std::string name);

	void ProcessEvents();
		
private:
	bool mIsPaused = false;
	bool mIsBossSpawned = false;

	sf::Sprite mMapSprite;
	sf::Texture mMapTexture;

	sf::View& mCamera;
	sf::RenderWindow& mWindow;
	SystemManager mSystemManager;
	EntityManager mEntityManager;

	void InitSystems();
	void InitPlayer();
	void InitMap();
};