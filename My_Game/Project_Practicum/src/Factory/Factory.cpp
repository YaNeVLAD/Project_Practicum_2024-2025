#include "Factory.h"

#include "../Entity/Weapon/Weapon.h"
#include "../Entity/Weapon/Book/Book.h"
#include "../Manager/System/SystemManager.h"
#include "../Manager/Entity/EntityManager.h"
#include "../Manager/Texture/TextureManager.h"
#include "../Entity/Weapon/Fireball/Fireball.h"
#include "../Entity/Weapon/BossWeapon/BossWeapon.h"
#include "../Entity/Weapon/MagicCharge/MagicCharge.h"
#include "../Entity/Weapon/LightningStrike/LightningStrike.h"

void Factory::InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera, bool& isBossSpawned)
{
	systemManager.AddSystem<InputSystem>();
	systemManager.AddSystem<WeaponSystem>();
	systemManager.AddSystem<MovementSystem>();
	systemManager.AddSystem<CollisionSystem>();
	systemManager.AddSystem<LifetimeSystem>(camera);
	systemManager.AddSystem<HomingProjectileSystem>();
	systemManager.AddSystem<OrbitalProjectileSystem>();
	systemManager.AddSystem<SpawnSystem>(camera, 1.0f, 1.0f, isBossSpawned);
	systemManager.AddSystem<TrailSystem>();

	systemManager.AddSystem<RenderSystem>(window);
	systemManager.AddSystem<CameraSystem>(camera);
	systemManager.AddSystem<HUDSystem>(window, camera);
}

std::unique_ptr<Weapon> Factory::CreateWeapon(const std::string& weaponName)
{
	if (weaponName == "Holy Book")
	{
		return std::make_unique<Book>();
	}
	if (weaponName == "Magic Charge")
	{
		return std::make_unique<MagicCharge>();
	}
	if (weaponName == "Fireball")
	{
		return std::make_unique<Fireball>();
	}
	if (weaponName == "Lightning Strike")
	{
		return std::make_unique<LightningStrike>();
	}

	throw std::invalid_argument("Unknown weapon type " + weaponName);
}

void Factory::CreateEnemy(EntityManager& entityManager, float x, float y)
{
	auto& enemy = entityManager.CreateEntity(EntityType::Enemy);
	enemy.AddComponent<TransformComponent>(x, y, 0.0f, 0.0f);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(50, 50));
	collisionShape->setOrigin(25, 25);
	enemy.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 15));
	enemy.AddComponent<HealthComponent>(50, 0.5f);

	std::vector<sf::Texture> frames = TextureManager::GetTextures("assets/melee_enemy/Walk.png", 128, 128);
	enemy.AddComponent<AnimationComponent>(frames, 0.2f, true, -1.0f, true);
	enemy.AddComponent<DrawableComponent>(frames[0], sf::Vector2f(0.75f, 0.75f));

	enemy.AddComponent<DamageComponent>(10, Player);
}

void Factory::CreateBoss(EntityManager& entityManager, float x, float y)
{
	auto& boss = entityManager.CreateEntity(EntityType::Enemy);

	boss.AddComponent<TransformComponent>(x, y, 0.0f, 0.0f);
	boss.AddComponent<BossHealthComponent>(300, 0.5f);
	boss.AddComponent<DamageComponent>(20, Player);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 80));
	collisionShape->setOrigin(20, 40);
	boss.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 15));

	boss.AddComponent<WeaponComponent>();
	auto weapons = boss.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_unique<BossWeapon>());

	std::vector<sf::Texture> frames = TextureManager::GetTextures("assets/boss/Walk.png", 128, 128);
	boss.AddComponent<AnimationComponent>(frames, 0.2f, true, -1.0f, true);
	boss.AddComponent<DrawableComponent>(frames[0], sf::Vector2f(1, 1));
}

void Factory::CreatePlayer(EntityManager& entityManager)
{
	auto& player = entityManager.CreateEntity(EntityType::Player);
	player.AddComponent<TransformComponent>(0.0f, 0.0f, 0.0f, 0.0f);
	player.AddComponent<InputComponent>();
	player.AddComponent<CameraComponent>();
	player.AddComponent<WeaponComponent>();

	auto weapons = player.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_unique<MagicCharge>());

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 40));
	collisionShape->setOrigin(20, 20);
	player.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 35));

	std::vector<sf::Texture> frames = TextureManager::GetTextures("assets/character/Walk.png", 128, 128);
	player.AddComponent<AnimationComponent>(frames, 0.2f, true, -1.0f, false);
	player.AddComponent<DrawableComponent>(frames[0], sf::Vector2f(1.1f, 1.1f));

	player.AddComponent<PlayerHealthComponent>(100, 1.0f);

	player.AddComponent<ExperienceComponent>(100);
}

void Factory::CreateHealthBonus(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& bonus = entityManager.CreateEntity(EntityType::Bonus);

	bonus.AddComponent<BonusComponent>(BonusComponent::BonusType::Health);

	bonus.AddComponent<TransformComponent>(pos.x, pos.y);
	bonus.AddComponent<DrawableComponent>(32, 32, sf::Color::Green);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	bonus.AddComponent<CollisionComponent>(std::move(collisionShape));

	bonus.AddComponent<LifetimeComponent>(15.f);
}

void Factory::CreateBombBonus(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& bonus = entityManager.CreateEntity(EntityType::Bonus);

	bonus.AddComponent<BonusComponent>(BonusComponent::BonusType::Bomb);

	bonus.AddComponent<DamageComponent>(999, Enemy);

	bonus.AddComponent<TransformComponent>(pos.x, pos.y);
	bonus.AddComponent<DrawableComponent>(32, 32, sf::Color::Red);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	bonus.AddComponent<CollisionComponent>(std::move(collisionShape));

	bonus.AddComponent<LifetimeComponent>(15.f);
}