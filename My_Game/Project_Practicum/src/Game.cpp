#include "Game.h"

#include <iostream>
#include "Factory/PlayerFactory/PlayerFactory.h"
#include "Factory/EnemyFactory/EnemyFactory.h"
#include "Factory/SystemFactory/SystemFactory.h"

Game::Game() : mWindow(sf::VideoMode::getDesktopMode(), "Game")
{
	mCamera = mWindow.getView();
	mWindow.setVerticalSyncEnabled(true);
	mWindow.setFramerateLimit(60);
}

void Game::InitSystems()
{
	SystemFactory::InitSystems(mSystemManager, mWindow, mCamera);
}

void Game::InitPlayer()
{
	PlayerFactory::Create(mEntityManager);
}

void Game::Run()
{
	sf::Clock clock;

	float fps = 0.0f;
	int frames = 0;
	sf::Clock fpsClock;

	while (mWindow.isOpen())
	{
		ProcessEvents();

		mWindow.setView(mCamera);

		float deltaTime = clock.restart().asSeconds();

		for (auto& system : mSystemManager.GetUpdateSystems())
		{
			system->Update(mEntityManager, deltaTime);
		}

		mWindow.clear();

		for (auto& system : mSystemManager.GetRenderSystems())
		{
			system->Render(mEntityManager, deltaTime);
		}

		mWindow.display();

		frames++;
		if (fpsClock.getElapsedTime().asSeconds() >= 1.0f)
		{
			fps = frames / fpsClock.getElapsedTime().asSeconds();
			fpsClock.restart();
			frames = 0;

			// Вывод FPS в консоль
			std::cout << "FPS: " << fps << "\n";
		}
	}
}

void Game::ProcessEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed: mWindow.close(); break;
		}
	}
}