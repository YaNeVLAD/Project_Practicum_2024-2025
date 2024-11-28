#pragma once

#include "SFML/Graphics.hpp"

class Entity;
enum EntityType;
class Weapon;

/**
* @brief Общая структура-прародитель для любых компонентов
*/
struct Component
{
	virtual ~Component() = default;
};

/**
* @brief Компонент отвечает за поведение фигуры при нажатии клавиш
*/
struct InputComponent : public Component {};

/**
* @brief Компонент отвечает за поведение камеры
*/
struct CameraComponent : public Component {};

/**
* @brief Компонент хранит положение сущности в пространстве
*/
struct TransformComponent : public Component
{
	/**
	* @brief Основной конструктор
	* @param float x - координата
	* @param float y - координата
	* @param float vx - скорость по оси x
	* @param float vy - скорость по оси y
	*/
	TransformComponent(float x = 0, float y = 0, float vx = 0, float vy = 0) : x(x), y(y), vx(vx), vy(vy) {}

	float x, y;
	float vx, vy;
	sf::Vector2f lastDirection = { 1.0f, 0.0f };
};

/**
* @brief Компонент хранит угол наклона сущности
*/

struct RotationComponent : public Component
{
	/**
	* @brief Основной конструктор
	* @param float angle - угол наклона сущности
	*/
	RotationComponent(float angle = 0.0f) : angle(angle) {}

	float angle;
};

/**
* @brief Компонент хранит коллизионную фигуру сущности
*/
struct CollisionComponent : public Component
{
	/**
	* @brief Основной конструктор
	* @param std::unique_ptr<sf::Shape> shape - указатель на фигуру коллизии
	* @param sf::Vector2f offset - смещение относительно центра сущности
	* @param float cooldown - перезарядка коллизии
	*/
	CollisionComponent(std::unique_ptr<sf::RectangleShape> shape, sf::Vector2f offset = sf::Vector2f(0.0f, 0.0f), float cooldown = 0.0f)
		: shape(std::move(shape)), offset(offset), cooldown(cooldown) {
	}

	std::unique_ptr<sf::RectangleShape> shape;
	sf::Vector2f offset;
	float elapsedTime = 0.0f;
	float cooldown;

	/**
	* @brief Метод для получения размеров зоны коллизии
	*/
	sf::FloatRect getRect() const
	{
		return shape->getGlobalBounds();
	}

	/**
	* @brief Метод для обновления позиции зоны коллизии
	* @param float x - координата x
	* @param float y - координата y
	*/
	void UpdatePosition(float x, float y) const
	{
		shape->setPosition(x + offset.x, y + offset.y);
	}
};

/**
* @brief Компонент отвечает за вид объекта на сцене
* @brief Может быть создан без текстуры, в таком случае необходимо задать размер и цвет прямоугольника
*/
struct DrawableComponent : public Component
{
	/**
	* @brief Конструктор для текстуры
	* @param const sf::Texture& texture - опциональный, компонент будет отображать переданную текстуру
	* @param sf::Vector2f scale - масштаб объекта
	*/
	DrawableComponent(const sf::Texture& texture, sf::Vector2f scale = { 1, 1 }) : texture(texture)
	{
		sprite.setTexture(texture);
		sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
		sprite.setScale(scale.x, scale.y);
	}

	/**
	* @brief Конструктор для цветного прямоугольника
	* @param int width - ширина прямоугольника
	* @param int height - высота прямоугольника
	* @param sf::Color color - цвет прямоугольника
	*/
	DrawableComponent(int width = 128, int height = 128, sf::Color color = sf::Color::White)
	{
		if (texture.create(width, height))
		{
			sf::Image image;
			image.create(width, height, color);
			texture.loadFromImage(image);
			sprite.setTexture(texture);
			sprite.setOrigin(width / 2.0f, height / 2.0f);
		}
	}

	sf::Sprite sprite;
	sf::Texture texture;
};

/**
* @brief Компонент хранит переданное оружие
*/
struct WeaponComponent : public Component
{
	/**
	* @brief Функция добавляет оружие в список оружия сущности
	*/
	void AddWeapon(std::unique_ptr<Weapon> weapon);
	std::vector<std::unique_ptr<Weapon>> weapons;
};

/**
* @brief Компонент обозначает что сущность имеет время жизни и будет удалена по его окончанию
*/
struct LifetimeComponent : public Component
{
	/**
	* @brief Основной конструктор
	* @param float maxLifetime - максимальное время жизни, по истечению которого сущность будет удалена
	*/
	LifetimeComponent(float maxLifeTime) : maxLifeTime(maxLifeTime), time(maxLifeTime) {}

	float time = 0.0f;
	float maxLifeTime = 0.0f;
};

struct HomingProjectileComponent : public Component
{
	HomingProjectileComponent(float speed)
		: speed(speed) {
	}

	float speed;
};

/**
* @brief Компонент обозначает что сущность будет вращаться по орбите вокруг другой сущности
*/
struct OrbitalProjectileComponent : public Component
{
	/**
	* @brief Основной конструктов
	* @param float radius - радиус орбиты
	* @param float speed - угловая скорость
	* @param TransformComponent* parent - указатель на TransformComponent родителя
	*/
	OrbitalProjectileComponent(float radius, float speed, TransformComponent* parent)
		: orbitRadius(radius), orbitSpeed(speed), parentTransform(parent) {
	}

	float orbitRadius = 0.0f;
	float orbitSpeed = 0.0f;
	float angle = 0.0f;
	TransformComponent* parentTransform;
};

/**
* @brief Компонент хранит данные, необходимые для анимации спрайта
*/
struct AnimationComponent : public Component
{
	/**
	* @brief Основной конструктор
	* @param std::vector<sf::Texture> frames - набор картинок для покадровой анимации
	* @param float frameTime - время отображения каждого кадра
	* @param bool loop - флаг, указывает на необходимость зациклить анимацию
	* @param float duration - время всей анимации
	*/
	AnimationComponent(
		std::vector<sf::Texture> frames,
		float frameTime,
		bool loop = true,
		float duration = -1.0f,
		bool isAnimating = false
	) : frames(std::move(frames)), frameTime(frameTime), loop(loop), duration(duration), isAnimating(isAnimating) {
	}

	std::vector<sf::Texture> frames;
	float frameTime = 0.0f;
	float elapsedTime = 0.0f;
	int currentFrameIndex = 0;
	bool loop = true;
	float duration = -1.0f;
	bool isAnimating = false;
};

/**
* @brief Компонент хранит данные, необходимые для получения урона по здоровью
*/
struct HealthComponent : public Component
{
	/**
	* @brief Основной конструктов
	* @param int maxHealth - максимальное здоровье сущности
	* @param float damageCooldown - перезарядка для получения урона
	*/
	HealthComponent(int maxHealth, float damageCooldown)
		: maxHealth(maxHealth), currentHealth(maxHealth), damageCooldown(damageCooldown) {
	}

	int maxHealth;
	int currentHealth;
	float damageCooldown;
	float cooldownTimer = 0.0f;

	void TryTakeDamage(int damage)
	{
		if (cooldownTimer <= 0.0f)
		{
			currentHealth -= damage;
			cooldownTimer = damageCooldown;
			if (currentHealth < 0)
			{
				currentHealth = 0;
			}
		}
	}

	void UpdateCooldown(float deltaTime)
	{
		if (cooldownTimer > 0.0f)
		{
			cooldownTimer -= deltaTime;
		}
	}

	bool IsAlive() const
	{
		return currentHealth > 0;
	}

	void LevelUp(int additionalHealth)
	{
		float healthRatio = static_cast<float>(currentHealth) / maxHealth;
		maxHealth += additionalHealth;
		currentHealth = static_cast<int>(maxHealth * healthRatio);
	}
};

/**
* @brief Компонент хранит данные, необходимые для получения урона по здоровью босса
*/
struct PlayerHealthComponent : public HealthComponent
{
	/**
	* @brief Основной конструктов
	* @param int maxHealth - максимальное здоровье сущности
	* @param float damageCooldown - перезарядка для получения урона
	*/
	PlayerHealthComponent(int maxHealth, float damageCooldown)
		: HealthComponent(maxHealth, damageCooldown) {
	}
};

/**
* @brief Компонент хранит данные, необходимые для получения урона по здоровью игрока
*/
struct BossHealthComponent : public HealthComponent
{
	/**
	* @brief Основной конструктов
	* @param int maxHealth - максимальное здоровье сущности
	* @param float damageCooldown - перезарядка для получения урона
	*/
	BossHealthComponent(int maxHealth, float damageCooldown)
		: HealthComponent(maxHealth, damageCooldown) {
	}
};

/**
* @brief Компонент показывает, сколько урона наносит сущность
*/
struct DamageComponent : public Component
{
	/**
	* @brief Основной конструктов
	* @param int amount - количество наносимого урона
	* @param EntityType targetType - тип сущности, которой будет наноситься урон
	*/
	DamageComponent(int amount, EntityType targetType) : amount(amount), targetType(targetType) {}

	int amount = 0;

	EntityType targetType;
};

/**
* @brief Компонент хранит данные, необходимые для получения опыта сущности
*/
struct ExperienceComponent : public Component
{
	/**
	* @brief Основной конструктов
	* @param int maxExperience - максимальное количество опыта на первом уровне
	*/
	ExperienceComponent(int maxExperience)
		: maxExperience(maxExperience) {
	}

	const float XP_PER_LVL_MULT = 1.5f;

	int maxExperience = 0;
	int currentExperience = 0;
	int level = 1;
	bool levelUpFlag = false;

	/**
	* @brief Функция добавления опыта
	* @param int xpAmount - количество полученного опыта
	*/
	void GainExperience(int xpAmount)
	{
		currentExperience += xpAmount;
	}
	
	/**
	* @brief Функция повышения уровня
	*/
	bool CheckLevelUp()
	{
		if (currentExperience >= maxExperience)
		{
			currentExperience = 0;
			maxExperience = static_cast<int>(maxExperience * XP_PER_LVL_MULT);
			level++;
			levelUpFlag = true;
			return true;
		}
		return false;
	}
};

struct TrailComponent : public Component
{
	TrailComponent(int damage, float interval) : damage(damage), spawnInterval(interval) {}

	int damage;
	float spawnInterval;
	float elapsedTime = 0.0f;
};