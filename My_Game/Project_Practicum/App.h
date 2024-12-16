#pragma once

#include "SFML/Graphics.hpp"

#include "src/Game.h"
#include "ui/Screen/Screen.h"

class App
{
public:
	App() :
		mWindow(sf::VideoMode::getDesktopMode(), "Bloody Survivors"),
		mCamera(mWindow.getView()),
		mGame(mWindow, mCamera),
		mCurrentState(AppState::MainMenu)
	{
		mWindow.setVerticalSyncEnabled(true);
		mWindow.setFramerateLimit(120);
		LoadFont();
		LoadTextures();
	}

	void Run();

private:
	enum class AppState
	{
		Defeat,
		Playing,
		Victory,
		MainMenu,
		WeaponUpgrade,
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

	void LoadFont();
	void LoadTextures();
};