#pragma once

#include "SFML/Graphics.hpp"

#include "src/Game.h"
#include "ui/Screen/Screen.h"

class App
{
public:
	App()
		: mWindow(sf::VideoMode::getDesktopMode(), "Bloody Survivors", sf::Style::Fullscreen),
		  mCamera(mWindow.getView()),
		  mGame(mWindow, mCamera),
		  mCurrentState(AppState::MainMenu)
	{
		mWindow.setVerticalSyncEnabled(true);
		mWindow.setFramerateLimit(60);
	}

	void Run();

private:
	enum class AppState
	{
		MainMenu,
		Playing,
		Victory,
		Defeat,
		WeaponUpgrade
	};

	std::vector<std::string> mAvailableWeapons;

	sf::RenderWindow mWindow;
	sf::View mCamera;
	AppState mCurrentState;
	Game mGame;

	sf::Clock mClock;

	sf::Font mFont;

	Screen mScreen;

	void ProcessEvents();
	void Update(float deltaTime);
	void Render(float deltaTime);

	void InitUpgradeScreen();
	void InitMainMenuScreen();
	void InitVictoryScreen();
	void InitDefeatScreen();
};