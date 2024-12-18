#pragma once

#include "SFML/Graphics.hpp"

#include "src/Game.h"
#include "ui/Screen/Screen.h"

class App
{
public:
	static App& Instance()
	{
		static App instance;
		return instance;
	}

	void Run();

	sf::RenderWindow& GetWindow() { return mWindow; }
	Game& GetGame() { return mGame; }
	Screen& GetScreen() { return mScreen; }

private:
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

	enum class AppState
	{
		Defeat,
		Playing,
		Victory,
		MainMenu,
		GameSetup,
		WeaponUpgrade,
	};

	~App() = default;
	App(const App&) = delete;
	App& operator=(const App&) = delete;

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

	void RenderDefeatScreen();
	void RenderVictoryScreen();
	void RenderUpgradeScreen();
	void RenderMainMenuScreen();
	void RenderGameSetupScreen();

	void LoadFont();
	void LoadTextures();
};