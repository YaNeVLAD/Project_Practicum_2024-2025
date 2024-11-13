#include "Game.h"
#include "Bow.h"

Game::Game() : mWindow(sf::VideoMode::getDesktopMode(), "Game") {}

void Game::initSystems()
{
	mSystemManager.AddSystem<RenderSystem>(mWindow);
	mSystemManager.AddSystem<MovementSystem>();
	mSystemManager.AddSystem<InputSystem>();
	mSystemManager.AddSystem<CameraSystem>(mWindow);
	mSystemManager.AddSystem<WeaponSystem>();
	mSystemManager.AddSystem<ProjectileSystem>();
}

void Game::initPlayer()
{
	auto& player = mEntityManager.CreateEntity();
	player.AddComponent<TransformComponent>(0, 0, 0, 0);
	player.AddComponent<DrawableComponent>(50, sf::Color::Red);
	player.AddComponent<InputComponent>();
	player.AddComponent<CameraComponent>();
	player.AddComponent<WeaponComponent>();
	auto weapons = player.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_unique<Bow>());
}

void Game::initEnemy()
{
	auto& enemy = mEntityManager.CreateEntity();
	enemy.AddComponent<TransformComponent>(100, 100, 0, 0);
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
