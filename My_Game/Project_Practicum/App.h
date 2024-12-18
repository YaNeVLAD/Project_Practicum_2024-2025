#pragma once

#include "SFML/Graphics.hpp"

#include "src/Game.h"
#include "ui/Screen/Screen.h"

class App
{
public:
	enum class State
	{
		Defeat,
		Playing,
		Victory,
		MainMenu,
		GameSetup,
		WeaponUpgrade,
	};

	static App& Instance()
	{
		static App instance;
		return instance;
	}

	void Run();

	sf::RenderWindow window;
	sf::View camera;

	State state;
	Game game;
	Screen screen;

private:
	App() :
		window(sf::VideoMode::getDesktopMode(), "Bloody Survivors"),
		camera(window.getView()),
		game(window, camera),
		state(State::MainMenu)
	{
		window.setVerticalSyncEnabled(true);
		window.setFramerateLimit(120);
		LoadFont();
		LoadTextures();
	}

	~App() = default;
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	std::vector<std::string> mAvailableWeapons;

	sf::Clock mClock;

	sf::Font mFont;

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