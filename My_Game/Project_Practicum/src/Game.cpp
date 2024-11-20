#include "Game.h"

#include "Factory/PlayerFactory/PlayerFactory.h"
#include "Factory/EnemyFactory/EnemyFactory.h"
#include "Factory/SystemFactory/SystemFactory.h"

void Game::InitSystems()
{
	SystemFactory::InitSystems(mSystemManager, mWindow, mCamera);
}

void Game::InitPlayer()
{
	PlayerFactory::Create(mEntityManager);
}

void Game::RunFrame(float deltaTime)
{
	for (auto& system : mSystemManager.GetUpdateSystems())
	{
		system->Update(mEntityManager, deltaTime);
	}
}

void Game::Render(float deltaTime)
{
	mWindow.setView(mCamera);
	for (auto& system : mSystemManager.GetRenderSystems())
	{
		system->Render(mEntityManager, deltaTime);
	}
}

bool Game::IsPlayerDefeated()
{
	auto player = mEntityManager.GetEntitiesWithComponents<PlayerHealthComponent>();
	if (player.size())
	{
		auto health = player.front()->GetComponent<PlayerHealthComponent>();
		return !health->IsAlive();
	}
	return true;
}

void Game::ProcessEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
	}
}