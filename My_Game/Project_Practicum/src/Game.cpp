#include "Game.h"
#include "entity/weapon/bow/Bow.h"
#include "../utils/SpriteSheet.h"

Game::Game() : mWindow(sf::VideoMode::getDesktopMode(), "Game")
{
	mCamera = mWindow.getView();
}

void Game::InitSystems()
{
	mSystemManager.AddSystem<RenderSystem>(mWindow);
	mSystemManager.AddSystem<MovementSystem>();
	mSystemManager.AddSystem<InputSystem>();
	mSystemManager.AddSystem<CameraSystem>(mCamera);
	mSystemManager.AddSystem<WeaponSystem>();
	mSystemManager.AddSystem<ProjectileSystem>(mCamera);
}

void Game::InitPlayer()
{
	auto& player = mEntityManager.CreateEntity();
	player.AddComponent<TransformComponent>(0.0f, 0.0f, 0.0f, 0.0f);
	player.AddComponent<DrawableComponent>(128, 128, sf::Color::White);
	player.AddComponent<InputComponent>();
	player.AddComponent<CameraComponent>();
	player.AddComponent<WeaponComponent>();
	auto weapons = player.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_unique<Bow>());

	std::vector<sf::Texture> frames = SpriteSheet::LoadTextures("assets/character/walk.png", 128, 128);
	std::cout << frames.size();
	player.AddComponent<AnimationComponent>(frames, 0.2f, true, -1.0f);
}

void Game::InitEnemy()
{
	auto& enemy = mEntityManager.CreateEntity();
	enemy.AddComponent<TransformComponent>(100.0f, 100.0f, 0.0f, 0.0f);
	enemy.AddComponent<DrawableComponent>(40, 40, sf::Color::Green);
}

void Game::Run()
{
	sf::Clock clock;

	while (mWindow.isOpen())
	{
		ProcessEvents();

		mWindow.setView(mCamera);

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
