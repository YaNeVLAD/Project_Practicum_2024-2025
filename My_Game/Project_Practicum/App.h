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
		mWindow.setFramerateLimit(60);
	}

	void Run();

private:
	enum AppState
	{
		MainMenu,
		Playing,
		Defeat,
	};

	sf::RenderWindow mWindow;
	sf::View mCamera;
	AppState mCurrentState;
	Game mGame;

	sf::Clock mClock;

	sf::Text mMainMenuText;
	sf::Text mDefeatText;
	sf::RectangleShape mExitButton;

	void ProcessEvents();
	void Update(float deltaTime);
	void Render(float deltaTime);

	void HandleMainMenuEvents(const sf::Event& event);
	void HandleDefeatScreenEvents(const sf::Event& event);

	void RenderMainMenu();
	void RenderDefeatScreen();
};