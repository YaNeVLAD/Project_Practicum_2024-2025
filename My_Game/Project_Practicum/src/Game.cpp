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
	mSystemManager.AddSystem<LifetimeSystem>(mCamera);
	mSystemManager.AddSystem<OrbitalProjectileSystem>();
	mSystemManager.AddSystem<CollisionSystem>();
}

void Game::InitPlayer()
{
	auto& player = mEntityManager.CreateEntity(EntityType::Player);
	player.AddComponent<TransformComponent>(0.0f, 0.0f, 0.0f, 0.0f);
	player.AddComponent<InputComponent>();
	player.AddComponent<CameraComponent>();
	player.AddComponent<WeaponComponent>();

	auto weapons = player.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_unique<MagicCharge>());
	weapons->AddWeapon(std::make_unique<Book>());
	
	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(45, 75));
	collisionShape->setOrigin(22.5, 37.5);
	player.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 35));

	std::vector<sf::Texture> frames = SpriteSheet::LoadTextures("assets/character/Walk.png", 128, 128);
	player.AddComponent<AnimationComponent>(frames, 0.2f, true, -1.0f, false);
	player.AddComponent<DrawableComponent>(frames[0], sf::Vector2f(1.1f, 1.1f));
}

void Game::InitEnemy()
{
	auto& enemy = mEntityManager.CreateEntity(EntityType::Enemy);
	enemy.AddComponent<TransformComponent>(100.0f, 100.0f, 0.0f, 0.0f);
	enemy.AddComponent<DrawableComponent>(40, 40, sf::Color::Red);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 40));
	collisionShape->setOrigin(20.0f, 20.0f);
	enemy.AddComponent<CollisionComponent>(std::move(collisionShape));
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

		mWindow.clear(sf::Color::White);

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