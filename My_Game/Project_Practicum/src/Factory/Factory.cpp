#include "Factory.h"

#include <random>

#include "../Entity/Weapon/Weapon.h"
#include "../Entity/Weapon/Axe/Axe.h"
#include "../Entity/Weapon/Book/Book.h"
#include "../Manager/System/SystemManager.h"
#include "../Manager/Entity/EntityManager.h"
#include "../Manager/Texture/TextureManager.h"
#include "../Entity/Weapon/Fireball/Fireball.h"
#include "../Entity/Weapon/BossWeapon/BossWeapon.h"
#include "../Entity/Weapon/MagicCharge/MagicCharge.h"
#include "../Entity/Weapon/LightningStrike/LightningStrike.h"

std::vector<std::string> names = { 
	"Влад", "Кирилл", "Максим", "Дима", "Илья", "Елисей", 
	"Степан", "Мирослав", "Данил", "Константин", "Михаил", 
	"Богдан", "Андрей", "Денис", "Антон", "Александр", "Тимофей", 
	"Григорий", "Арсений", "Владимир"
};

std::vector<std::string> titles = {
	"Повелитель", "Хранитель", "Защитник", "Мастер", "Поборник", "Убийца",
	"Созидатель", "Ценитель", "Гений", "Король", "Воин", "Покоритель", "Титан",
	"Звезда", "Пророк", "Исцелитель", "Легенда", "Странник", "Владыка",
	"Мудрец", "Строитель", "Чародей", "Охотник", "Скиталец", "Чемпион"
};

std::vector<std::string> objects = {
	"Ночи", "Мечей", "Мудрости", "Тьмы", "Ветров", "Травы", "Огня", "Дыма",
	"С++", "Жизни", "Смерти", "Молнии", "Теней", "Света", "Льда", "Железа",
	"Лавы", "Водопадов", "Старого мира", "Леса", "Песков", "Скалы", "Древности",
	"Чудес", "Травы", "Судьбы", "Души", "Пламени", "Звезд", "Коллизий"
};

std::random_device rd;
std::mt19937 rg(rd());
std::uniform_int_distribution<size_t> nd(0, names.size() -1);
std::uniform_int_distribution<size_t> td(0, titles.size() -1);
std::uniform_int_distribution<size_t> od(0, objects.size() -1);

static std::unordered_map<std::string, std::function<std::shared_ptr<Weapon>()>> weaponCreators = {
		{ "Magic Charge", []() { return std::make_shared<MagicCharge>(); } },
		{ "Holy Book", []() { return std::make_shared<Book>(); } },
		{ "Fireball", []() { return std::make_shared<Fireball>(); } },
		{ "Lightning Strike", []() { return std::make_shared<LightningStrike>(); } },
		{ "Axe", []() { return std::make_shared<Axe>(); } },
};

void Factory::InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera, size_t* defeatedBosses, size_t* maxBosses, bool& isPaused)
{
	systemManager.AddSystem<InputSystem>();
	systemManager.AddSystem<WeaponSystem>();
	systemManager.AddSystem<MovementSystem>();
	systemManager.AddSystem<CollisionSystem>();
	systemManager.AddSystem<LifetimeSystem>(camera, defeatedBosses);
	systemManager.AddSystem<HomingProjectileSystem>();
	systemManager.AddSystem<OrbitalProjectileSystem>();
	systemManager.AddSystem<SpawnSystem>(camera, 0.3f, 1.f, 2.f, maxBosses);
	systemManager.AddSystem<TrailSystem>();
	systemManager.AddSystem<DamageSystem>();
	systemManager.AddSystem<ContainerSystem>();
	systemManager.AddSystem<DeathAnimationSystem>(camera, isPaused, *maxBosses == 1);

	systemManager.AddSystem<RenderSystem>(window);
	systemManager.AddSystem<CameraSystem>(camera);
	systemManager.AddSystem<HUDSystem>(window, camera);
}

std::shared_ptr<Weapon> Factory::CreateWeapon(const std::string& name)
{
	auto it = weaponCreators.find(name);
	if (it != weaponCreators.end())
	{
		return it->second();
	}

	return nullptr;
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

	enemy.AddComponent<DamageComponent>(1, 0.1f, Player);
}

void Factory::CreateBoss(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& boss = entityManager.CreateEntity(EntityType::Enemy);

	boss.AddComponent<TransformComponent>(pos);
	boss.AddComponent<BossHealthComponent>(300);
	boss.AddComponent<DamageComponent>(2, 0.1f, Player);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 80));
	collisionShape->setOrigin(20, 40);
	boss.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 15));

	boss.AddComponent<WeaponComponent>();
	auto weapons = boss.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_unique<BossWeapon>());

	boss.AddComponent<AnimationComponent>(0.2f, true);

	std::vector<sf::Texture> walkFrames = TextureManager::GetTextures("assets/boss/Walk.png", 128, 128);
	std::vector<sf::Texture> hurtFrames = TextureManager::GetTextures("assets/boss/Hurt.png", 128, 128);
	std::vector<sf::Texture> deadFrames = TextureManager::GetTextures("assets/boss/Dead.png", 128, 128);

	auto animation = boss.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::WALK, walkFrames);
	animation->AddAnimation(AnimationComponent::HURT, hurtFrames);
	animation->AddAnimation(AnimationComponent::DEAD, deadFrames);
	animation->AddAnimation(AnimationComponent::IDLE, deadFrames);

	boss.AddComponent<DrawableComponent>(walkFrames[0], sf::Vector2f(1, 1));

	boss.AddComponent<NameComponent>(CreateRandomName());
}

void Factory::CreatePlayer(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& player = entityManager.CreateEntity(EntityType::Player);
	player.AddComponent<TransformComponent>(pos);
	player.AddComponent<InputComponent>();
	player.AddComponent<CameraComponent>();
	player.AddComponent<WeaponComponent>();

	auto weapons = player.GetComponent<WeaponComponent>();
	weapons->AddWeapon(std::make_shared<Axe>());

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

	player.AddComponent<AbilityComponent>(AbilityComponent::Haste, 3.f, 1.f, sf::Keyboard::Key::E);
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

	bonus.AddComponent<TransformComponent>(pos);
	bonus.AddComponent<DrawableComponent>(32, 32, sf::Color::Red);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	bonus.AddComponent<CollisionComponent>(std::move(collisionShape));

	bonus.AddComponent<LifetimeComponent>(30.f);
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

	magnet.AddComponent<LifetimeComponent>(30.f);
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

	experience.AddComponent<LifetimeComponent>(60.f);
}

std::string Factory::CreateRandomName()
{
	std::vector<std::string> namesCopy = names;
	std::vector<std::string> titlesCopy = titles;
	std::vector<std::string> objectsCopy = objects;

	std::shuffle(names.begin(), names.end(), rg);
	std::shuffle(titles.begin(), titles.end(), rg);
	std::shuffle(objects.begin(), objects.end(), rg);

	std::string randomName = names[nd(rg)];
	std::string randomTitle = titles[td(rg)];
	std::string randomObject = objects[od(rg)];

	return randomName + ", " + randomTitle + " " + randomObject;
}

void Factory::LoadTextures()
{
	TextureManager::GetFont("assets/font/Roboto-Bold.ttf");

	TextureManager::GetTextures("assets/weapon/Axe.png", 64, 64);
	TextureManager::GetTextures("assets/weapon/Book.png", 95, 128);
	TextureManager::GetTextures("assets/weapon/Charge.png", 64, 64);
	TextureManager::GetTextures("assets/weapon/Fireball.png", 64, 64);

	TextureManager::GetTextures("assets/melee_enemy/Walk.png", 128, 128);

	TextureManager::GetTextures("assets/boss/Walk.png", 128, 128);
	TextureManager::GetTextures("assets/boss/Hurt.png", 128, 128);
	TextureManager::GetTextures("assets/boss/Dead.png", 128, 128);

	TextureManager::GetTextures("assets/character/Walk.png", 128, 128);
	TextureManager::GetTextures("assets/character/Dead.png", 128, 128);
	TextureManager::GetTextures("assets/character/Idle.png", 128, 128);
	TextureManager::GetTextures("assets/character/Hurt.png", 128, 128);

	TextureManager::GetTextures("assets/map/Barrel.png", 128, 128);
}