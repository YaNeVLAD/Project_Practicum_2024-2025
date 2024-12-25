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
#include "../Config/GameConfig.h"
#include "../Game.h"

std::vector<std::string> names = {
	"Влад", "Кирилл", "Максим", "Дима", "Илья", "Елисей",
	"Степан", "Мирослав", "Данил", "Константин", "Михаил",
	"Богдан", "Андрей", "Денис", "Антон", "Александр", "Тимофей",
	"Григорий", "Арсений", "Владимир", "Иван", "Глеб"
};

std::vector<std::string> titles = {
	"Повелитель", "Хранитель", "Защитник", "Мастер", "Поборник", "Убийца",
	"Созидатель", "Ценитель", "Гений", "Король", "Воин", "Покоритель", "Титан",
	"Звезда", "Пророк", "Исцелитель", "Легенда", "Странник", "Владыка",
	"Мудрец", "Строитель", "Чародей", "Охотник", "Скиталец", "Чемпион", "Лидер"
};

std::vector<std::string> objects = {
	"Ночи", "Мечей", "Мудрости", "Тьмы", "Ветров", "Травы", "Огня", "Дыма",
	"С++", "Жизни", "Смерти", "Молнии", "Теней", "Света", "Льда", "Железа",
	"Лавы", "Водопадов", "Старого мира", "Леса", "Песков", "Скалы", "Древности",
	"Чудес", "Травы", "Судьбы", "Души", "Пламени", "Звезд", "Коллизий", "Мобилок",
	"Бекенда", "Фронтенда", "УПО"
};

std::random_device rd;
std::mt19937 rg(rd());
std::uniform_int_distribution<size_t> nd(0, names.size() - 1);
std::uniform_int_distribution<size_t> td(0, titles.size() - 1);
std::uniform_int_distribution<size_t> od(0, objects.size() - 1);

static std::unordered_map<std::string, std::function<std::shared_ptr<Weapon>()>> weaponCreators = {
		{ "Magic Charge", []() { return std::make_shared<MagicCharge>(); } },
		{ "Holy Book", []() { return std::make_shared<Book>(); } },
		{ "Fireball", []() { return std::make_shared<Fireball>(); } },
		{ "Lightning Strike", []() { return std::make_shared<LightningStrike>(); } },
		{ "Axe", []() { return std::make_shared<Axe>(); } },
};

void Factory::InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera, bool& isPaused)
{
	systemManager.AddSystem<InputSystem>();
	systemManager.AddSystem<WeaponSystem>();
	systemManager.AddSystem<MovementSystem>();
	systemManager.AddSystem<CollisionSystem>();
	systemManager.AddSystem<LifetimeSystem>(camera);
	systemManager.AddSystem<HomingProjectileSystem>();
	systemManager.AddSystem<OrbitalProjectileSystem>();
	systemManager.AddSystem<SpawnSystem>(camera, 0.2f, 15.f, 60.f);
	systemManager.AddSystem<TrailSystem>();
	systemManager.AddSystem<DamageSystem>();
	systemManager.AddSystem<ContainerSystem>();
	systemManager.AddSystem<DeathAnimationSystem>(camera, isPaused, GameConfig::GetInstance()->maxBosses == 1);

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
	size_t multiplier = static_cast<size_t>(1ull) << GameConfig::GetInstance()->killedBosses;

	auto& enemy = entityManager.CreateEntity(EntityType::Enemy);
	enemy.AddComponent<TransformComponent>(pos);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 40));
	collisionShape->setOrigin(20, 20);
	enemy.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 10));
	enemy.AddComponent<HealthComponent>(50 * multiplier);

	enemy.AddComponent<AnimationComponent>(0.2f, true);

	auto walkFrames = TextureManager::GetTextures("assets/melee_enemy/Walk.png", 128, 128);

	auto animation = enemy.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::WALK, walkFrames);

	enemy.AddComponent<DrawableComponent>(walkFrames->at(0), sf::Vector2f(0.6f, 0.6f));

	enemy.AddComponent<DamageComponent>(1, 0.1f, Player);
}

void Factory::CreateBoss(EntityManager& entityManager, sf::Vector2f pos)
{
	size_t multiplier = static_cast<size_t>(1ull) << GameConfig::GetInstance()->killedBosses;

	auto& boss = entityManager.CreateEntity(EntityType::Enemy);

	boss.AddComponent<TransformComponent>(pos);
	boss.AddComponent<BossHealthComponent>(300 * multiplier);
	boss.AddComponent<DamageComponent>(2, 0.1f, Player);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 80));
	collisionShape->setOrigin(20, 40);
	boss.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 15));

	boss.AddComponent<WeaponComponent>(std::make_shared<BossWeapon>());

	boss.AddComponent<AnimationComponent>(0.2f, true);

	auto walkFrames = TextureManager::GetTextures("assets/boss/Walk.png", 128, 128);
	auto hurtFrames = TextureManager::GetTextures("assets/boss/Hurt.png", 128, 128);
	auto deadFrames = TextureManager::GetTextures("assets/boss/Dead.png", 128, 128);

	auto animation = boss.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::WALK, walkFrames);
	animation->AddAnimation(AnimationComponent::HURT, hurtFrames);
	animation->AddAnimation(AnimationComponent::DEAD, deadFrames);
	animation->AddAnimation(AnimationComponent::IDLE, deadFrames);

	boss.AddComponent<DrawableComponent>(walkFrames->at(0), sf::Vector2f(1, 1));

	boss.AddComponent<NameComponent>(CreateRandomName());
}

using PlayerType = GameConfig::PlayerType;
void Factory::CreatePlayer(EntityManager& entityManager, sf::Vector2f pos)
{
	auto type = GameConfig::GetInstance()->playerType;
	auto& player = entityManager.CreateEntity(EntityType::Player);
	player.AddComponent<TransformComponent>(pos);
	player.AddComponent<InputComponent>();
	player.AddComponent<CameraComponent>();

	player.AddComponent<WeaponComponent>(CreateWeapon(type == PlayerType::First ? "Magic Charge" : "Axe"));

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40, 40));
	collisionShape->setOrigin(20, 20);
	player.AddComponent<CollisionComponent>(std::move(collisionShape), sf::Vector2f(0, 35));

	player.AddComponent<AnimationComponent>(0.2f, true);

	auto walkFrames = TextureManager::GetTextures("assets/character/Walk.png", 128, 128);
	auto idleFrames = TextureManager::GetTextures("assets/character/Idle.png", 128, 128);
	auto hurtFrames = TextureManager::GetTextures("assets/character/Hurt.png", 128, 128);
	auto deadFrames = TextureManager::GetTextures("assets/character/Dead.png", 128, 128);

	auto animation = player.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::IDLE, idleFrames);
	animation->AddAnimation(AnimationComponent::WALK, walkFrames);
	animation->AddAnimation(AnimationComponent::HURT, hurtFrames);
	animation->AddAnimation(AnimationComponent::DEAD, deadFrames);

	player.AddComponent<DrawableComponent>(idleFrames->at(0), sf::Vector2f(1.f, 1.f));

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

	auto frames = TextureManager::GetTextures("assets/map/Barrel.png", 128, 128);
	container.AddComponent<DrawableComponent>(frames->at(0), sf::Vector2f(0.75, 0.75));

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(32, 32));
	collisionShape->setOrigin(16, 16);
	container.AddComponent<CollisionComponent>(std::move(collisionShape));
}

void Factory::CreateExperience(EntityManager& entityManager, sf::Vector2f pos)
{
	auto& experience = entityManager.CreateEntity(EntityType::Particle);

	experience.AddComponent<ExperienceComponent>(50);

	experience.AddComponent<TransformComponent>(pos);

	auto frames = TextureManager::GetTextures("assets/gif/xp/Experience.png", 64, 64);
	experience.AddComponent<DrawableComponent>(frames->at(0), sf::Vector2f(0.4f, 0.4f));

	experience.AddComponent<AnimationComponent>(0.1f, true);
	auto animation = experience.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::IDLE, frames);
	animation->SetState(AnimationComponent::IDLE);

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

	TextureManager::GetTextures("assets/gif/xp/Experience.png", 64, 64);
}