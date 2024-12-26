#pragma once

#include "SFML/Graphics.hpp"
#include "../ui/Map/Map.h"
#include "Factory/Factory.h"
#include "Manager/System/SystemManager.h"
#include "Manager/Entity/EntityManager.h"
#include "Config/GameConfig.h"

class Game
{
public:
	using PlayerType = GameConfig::PlayerType;

	Game(sf::RenderWindow& window, sf::View& camera)
		: mWindow(window), mCamera(camera), mConfig(GameConfig::GetInstance()) {}

	void Init()
	{
		InitKeyBindings();
		InitSystems();
		InitPlayer();
		InitMap();
		LoadFont();
	}

	void Restart(size_t bossCount = -1)
	{
		Reset(bossCount);
		Init();
	}

	void Render(float deltaTime);
	void RunFrame(float deltaTime);

	void Pause();
	void Resume();
		
	bool IsBossDefeated() const;
	bool IsPlayerDefeated();
	bool HasPlayerLeveledUp();

	std::vector<std::shared_ptr<Weapon>> GetAvailableWeapons();
	void UpgradeWeapon(std::string name);

	void ProcessEvents(const sf::Event& event);

	PlayerType playerType = PlayerType::First;
private:
	GameConfig* mConfig;

	bool mIsPaused = false;

	Map mMap;

	sf::Font mFont;

	Screen mScreen;

	sf::View& mCamera;
	sf::RenderWindow& mWindow;
	SystemManager mSystemManager;
	EntityManager mEntityManager;

	bool CanPause();

	void Reset(size_t bossCount);

	void InitKeyBindings();
	void InitSystems();
	void InitPlayer();
	void InitMap();

	void LoadFont();

	void RenderPauseScreen();
};