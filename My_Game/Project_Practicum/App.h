#pragma once

#include "SFML/Graphics.hpp"

#include "src/Game.h"

class App
{
public:
	App()
		: mWindow(sf::VideoMode::getDesktopMode(), "Bloody Survivors"),
		  mCamera(mWindow.getView()),
		  mGame(mWindow, mCamera),
		  mCurrentState(MainMenu)
	{
		mWindow.setVerticalSyncEnabled(true);
		mWindow.setFramerateLimit(120);
	}

	void Run();

private:
	enum AppState
	{
		MainMenu,
		Playing,
		Defeat,
		WeaponUpgrade
	};

	sf::RenderWindow mWindow;
	sf::View mCamera;
	AppState mCurrentState;
	Game mGame;

	sf::Clock mClock;

	sf::Text mMainMenuText;
	sf::Text mDefeatText;
	sf::RectangleShape mExitButton;

	sf::Font mFont;
	std::vector<sf::Text> mButtonTexts;
	std::vector<sf::RectangleShape> mWeaponUpgradeButtons;
	sf::Text mUpgradePrompt;

	void ProcessEvents();
	void Update(float deltaTime);
	void Render(float deltaTime);

	void HandleMainMenuEvents(const sf::Event& event);
	void HandleWeaponUpgradeEvents(const sf::Event& event);
	void HandleDefeatScreenEvents(const sf::Event& event);

	void InitUpgradeScreen();

	void RenderMainMenu();
	void RenderDefeatScreen();
	void RenderWeaponUpgradeScreen();
};