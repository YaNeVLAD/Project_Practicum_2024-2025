#include "Game.h"
#include "Entity/Weapon/MagicCharge/MagicCharge.h"
#include "../utils/SpriteSheet.h"
#include "Entity/Weapon/Book/Book.h"

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
	mSystemManager.AddSystem<OrbitalProjectileSystem>();
}

void Game::InitPlayer()
{
	auto& player = mEntityManager.CreateEntity();
	player.AddComponent<TransformComponent>(0.0f, 0.0f, 0.0f, 0.0f);
	player.AddComponent<InputComponent>();
	player.AddComponent<CameraComponent>();
	player.AddComponent<WeaponComponent>();
	auto weapons = player.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_unique<MagicCharge>());
	weapons->AddWeapon(std::make_unique<Book>());

	std::vector<sf::Texture> frames = SpriteSheet::LoadTextures("assets/character/Walk.png", 128, 128);
	player.AddComponent<AnimationComponent>(frames, 0.2f, true, -1.0f, false);
	player.AddComponent<DrawableComponent>(frames[0], sf::Vector2f(1.5f, 1.5f));
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

		if (auto renderSystem = mSystemManager.GetSystem<RenderSystem>())
		{
			renderSystem->Update(mEntityManager, deltaTime);
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