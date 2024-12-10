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

void Factory::InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera, bool& isBossSpawned, bool& isPaused)
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
	systemManager.AddSystem<DamageSystem>();
	systemManager.AddSystem<ContainerSystem>();
	systemManager.AddSystem<DeathAnimationSystem>(camera, isPaused);

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

void Factory::CreateEnemy(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& enemy = entityManager.CreateEntity(EntityType::Enemy);
	enemy.AddComponent<TransformComponent>(pos);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(50, 50));
	collisionShape->setOrigin(25, 25);
	enemy.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 15));
	enemy.AddComponent<HealthComponent>(50);

	enemy.AddComponent<AnimationComponent>(0.2f, true);

	std::vector<sf::Texture> walkFrames = TextureManager::GetTextures("assets/melee_enemy/Walk.png", 128, 128);
	
	auto animation = enemy.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::WALK, walkFrames);

	enemy.AddComponent<DrawableComponent>(walkFrames[0], sf::Vector2f(0.75f, 0.75f));

	enemy.AddComponent<DamageComponent>(10, 1.f, Player);
}

void Factory::CreateBoss(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& boss = entityManager.CreateEntity(EntityType::Enemy);

	boss.AddComponent<TransformComponent>(pos);
	boss.AddComponent<BossHealthComponent>(300);
	boss.AddComponent<DamageComponent>(20, 1.f, Player);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 80));
	collisionShape->setOrigin(20, 40);
	boss.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 15));

	boss.AddComponent<WeaponComponent>();
	auto weapons = boss.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_unique<BossWeapon>());

	boss.AddComponent<AnimationComponent>(0.2f, true);

	std::vector<sf::Texture> walkFrames = TextureManager::GetTextures("assets/boss/Walk.png", 128, 128);
	std::vector<sf::Texture> hurtFrames = TextureManager::GetTextures("assets/boss/Hurt.png", 128, 128);

	auto animation = boss.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::WALK, walkFrames);
	animation->AddAnimation(AnimationComponent::HURT, hurtFrames);

	boss.AddComponent<DrawableComponent>(walkFrames[0], sf::Vector2f(1, 1));
}

void Factory::CreatePlayer(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& player = entityManager.CreateEntity(EntityType::Player);
	player.AddComponent<TransformComponent>(pos);
	player.AddComponent<InputComponent>();
	player.AddComponent<CameraComponent>();
	player.AddComponent<WeaponComponent>();

	auto weapons = player.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_unique<MagicCharge>());

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 40));
	collisionShape->setOrigin(20, 20);
	player.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 35));

	player.AddComponent<AnimationComponent>(0.2f, true);

	std::vector<sf::Texture> walkFrames = TextureManager::GetTextures("assets/character/Walk.png", 128, 128);
	std::vector<sf::Texture> idleFrames = TextureManager::GetTextures("assets/character/Idle.png", 128, 128);
	std::vector<sf::Texture> hurtFrames = TextureManager::GetTextures("assets/character/Hurt.png", 128, 128);
	std::vector<sf::Texture> deadFrames = TextureManager::GetTextures("assets/character/Dead.png", 128, 128);

	auto animation = player.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::IDLE, idleFrames);
	animation->AddAnimation(AnimationComponent::WALK, walkFrames);
	animation->AddAnimation(AnimationComponent::HURT, hurtFrames);
	animation->AddAnimation(AnimationComponent::DEAD, deadFrames);

	player.AddComponent<DrawableComponent>(idleFrames[0], sf::Vector2f(1.1f, 1.1f));

	player.AddComponent<PlayerHealthComponent>(100);

	player.AddComponent<LevelComponent>(100);
}

void Factory::CreateHealthBonus(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& bonus = entityManager.CreateEntity(EntityType::Bonus);

	bonus.AddComponent<BonusComponent>(BonusComponent::BonusType::Health);

	bonus.AddComponent<TransformComponent>(pos);
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

	bonus.AddComponent<DamageComponent>(999, 0.f, Enemy);

	bonus.AddComponent<TransformComponent>(pos);
	bonus.AddComponent<DrawableComponent>(32, 32, sf::Color::Red);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	bonus.AddComponent<CollisionComponent>(std::move(collisionShape));

	bonus.AddComponent<LifetimeComponent>(15.f);
}

void Factory::CreateMagnetBonus(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& magnet = entityManager.CreateEntity(EntityType::Bonus);

	magnet.AddComponent<BonusComponent>(BonusComponent::BonusType::Magnet);

	magnet.AddComponent<TransformComponent>(pos);
	magnet.AddComponent<DrawableComponent>(32, 32, sf::Color::Blue);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	magnet.AddComponent<CollisionComponent>(std::move(collisionShape));

	magnet.AddComponent<LifetimeComponent>(15.f);
}

void Factory::CreateContainer(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& container = entityManager.CreateEntity(EntityType::Enemy);

	container.AddComponent<ContainerComponent>();

	container.AddComponent<TransformComponent>(pos);

	auto& frames = TextureManager::GetTextures("assets/map/Barrel.png", 128, 128);
	container.AddComponent<DrawableComponent>(frames[0], sf::Vector2f(0.75, 0.75));

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	container.AddComponent<CollisionComponent>(std::move(collisionShape));
}

void Factory::CreateExperience(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& experience = entityManager.CreateEntity(EntityType::Particle);

	experience.AddComponent<ExperienceComponent>(50);

	experience.AddComponent<TransformComponent>(pos);

	experience.AddComponent<DrawableComponent>(16, 16, sf::Color::Green);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	experience.AddComponent<CollisionComponent>(std::move(collisionShape));

	experience.AddComponent<LifetimeComponent>(30.f);
}