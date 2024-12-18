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

	void Init()
	{
		InitKeyBindings();
		InitSystems();
		InitPlayer();
		InitMap();
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

	std::vector<std::string> GetAvailableWeapons();
	void UpgradeWeapon(std::string name);

	void ProcessEvents(const sf::Event& event);

	void ChangeMaxBosses(size_t offset);
	size_t GetMaxBosses() const { return mMaxBosses; }
	void SetIsInfinite(bool value) { mIsInfinite = value; }

private:
	bool mIsPaused = false;
	bool mIsInfinite = false;

	size_t mDefeatedBosses = 0;
	size_t mMaxBosses = 1;

	Map mMap;

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

	void RenderPauseScreen();
};