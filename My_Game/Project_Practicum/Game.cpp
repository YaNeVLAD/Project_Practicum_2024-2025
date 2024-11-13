#include "Game.h"

Game::Game() : mWindow(sf::VideoMode::getDesktopMode(), "Game")
{
	mSystemManager.AddSystem<RenderSystem>(mWindow);
	mSystemManager.AddSystem<MovementSystem>();
	mSystemManager.AddSystem<InputSystem>();
	mSystemManager.AddSystem<CameraSystem>(mWindow);

	auto& player = mEntityManager.CreateEntity();
	player.AddComponent<PositionComponent>(0, 0);
	player.AddComponent<VelocityComponent>(0, 0);
	player.AddComponent<DrawableComponent>(50, sf::Color::Red);
	player.AddComponent<InputComponent>();
	player.AddComponent<CameraComponent>();

	auto& enemy = mEntityManager.CreateEntity();
	enemy.AddComponent<PositionComponent>(100, 100);
	enemy.AddComponent<VelocityComponent>(0, 0);
	enemy.AddComponent<DrawableComponent>(40, sf::Color::Green);
}

void Game::Run()
{
	sf::Clock clock;

	while (mWindow.isOpen())
	{
		ProcessEvents();
		float deltaTime = clock.restart().asSeconds();

		for (auto& system : mSystemManager.GetSystems())
		{
			system->Update(mEntityManager, deltaTime);
		}

		mWindow.clear();

		for (auto& system : mSystemManager.GetSystems())
		{
			system->Update(mEntityManager, deltaTime);
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
