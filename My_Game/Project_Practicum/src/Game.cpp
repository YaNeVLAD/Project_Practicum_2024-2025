#include "Game.h"

#include "Factory/PlayerFactory/PlayerFactory.h"
#include "Factory/EnemyFactory/EnemyFactory.h"
#include "Factory/SystemFactory/SystemFactory.h"

Game::Game() : mWindow(sf::VideoMode::getDesktopMode(), "Game")
{
	mCamera = mWindow.getView();
}

void Game::InitSystems()
{
	SystemFactory::InitSystems(mSystemManager, mWindow, mCamera);
}

void Game::InitPlayer()
{
	PlayerFactory::Create(mEntityManager);
}

void Game::InitEnemy()
{
}

void Game::Run()
{
	sf::Clock clock;

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