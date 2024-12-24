#pragma once

#include "SFML/Graphics.hpp"

#include "src/Game.h"
#include "ui/Screen/Screen.h"
#include "ui/AnimatedSprite.hpp"
#include "src/Config/GameConfig.h"

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
		state(State::MainMenu),
		mConfig(GameConfig::GetInstance())
	{
		window.setVerticalSyncEnabled(true);
		window.setFramerateLimit(120);
		LoadFont();
		LoadTextures();
	}

	~App() = default;
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	GameConfig* mConfig;

	std::vector<std::shared_ptr<Weapon>> mAvailableWeapons;
	std::unordered_map<std::string, std::shared_ptr<AnimatedSprite>> mWeaponSprites;

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