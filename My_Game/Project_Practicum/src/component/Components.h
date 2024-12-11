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
	* @param sf::Vector2f position - вектор позиции
	* @param sf::Vector2f velocity - вектор скорости
	*/
	TransformComponent(sf::Vector2f position = { 0, 0 }, sf::Vector2f velocity = { 0, 0 })
		: x(position.x), y(position.y), vx(velocity.x), vy(velocity.y) {
	}

	/**
	* @brief Функция получения вектора позиции
	*/
	sf::Vector2f GetPosition()
	{
		return { x, y };
	}

	/**
	* @brief Функция получения вектора скорости
	*/
	sf::Vector2f GetVelocity()
	{
		return { vx, vy };
	}

	float x, y;
	float vx, vy;
	sf::Vector2f lastDirection = { 0.f, 0.f };
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

/**
* @brief Компонент обозначает что сущность будет наводиться на ближайшего врага
*/
struct HomingProjectileComponent : public Component
{
	/**
	* @brief Основной конструктор
	* @param float speed - скорость снаряда
	* @param EntityType targetType - тип сущности, на которые будет наводиться снаряд
	*/
	HomingProjectileComponent(float speed, EntityType targetType)
		: speed(speed), targetType(targetType) {
	}

	float speed;
	EntityType targetType;
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
	enum AnimationState
	{
		IDLE,
		WALK,
		HURT,
		DEAD,
		ATTACK
	};

	/**
	* @brief Основной конструктор
	* @param std::vector<sf::Texture> frames - набор картинок для покадровой анимации
	* @param float frameTime - время отображения каждого кадра
	* @param bool loop - флаг, указывает на необходимость зациклить анимацию
	* @param float duration - время всей анимации
	*/
	AnimationComponent(
		float frameTime,
		bool loop = true
	) : frameTime(frameTime), loop(loop), currentState(AnimationState::IDLE) {
	}

	float frameTime = 0.0f;
	float elapsedTime = 0.0f;
	int currentFrameIndex = 0;
	bool loop = true;

	AnimationState currentState;
	std::vector<sf::Texture>* frames = nullptr;
	std::map<AnimationState, std::vector<sf::Texture>> animations;

	void SetState(AnimationState state)
	{
		if (currentState != state && animations.find(state) != animations.end())
		{
			currentState = state;
			frames = &animations[state];
			currentFrameIndex = 0;
			elapsedTime = 0.f;
		}
	}

	void AddAnimation(AnimationState state, const std::vector<sf::Texture>& stateFrames)
	{
		animations[state] = stateFrames;
		if (currentState == state)
		{
			frames = &animations[state];
		}
	}

	const sf::Texture* GetCurrentFrame() const
	{
		if (frames != nullptr && !frames->empty() && currentFrameIndex < frames->size())
		{
			return &(*frames)[currentFrameIndex];
		}
		return nullptr;
	}
};

/**
* @brief Компонент хранит данные, необходимые для получения урона по здоровью
*/
struct HealthComponent : public Component
{
	/**
	* @brief Основной конструктов
	* @param int maxHealth - максимальное здоровье сущности
	*/
	HealthComponent(int maxHealth) : maxHealth(maxHealth), currentHealth(maxHealth) {}

	int maxHealth;
	int currentHealth;

	/**
	* @brief Функция вычитает здоровье из текущего
	* @param int amount - количество вычитаемого здоровья
	*/
	void RemoveHealth(int amount)
	{
		currentHealth -= amount;
	}

	/**
	* @brief Функция добавляет здоровье, не увеличивая его максимум
	* @param int amount - количество добавляемого здоровья
	*/
	void AddHealth(int amount)
	{
		(currentHealth + amount > maxHealth)
			? currentHealth = maxHealth
			: currentHealth += amount;
	}

	/**
	* @brief Функция определяет жива сущность или нет
	*/
	bool IsAlive() const
	{
		return currentHealth > 0;
	}

	/**
	* @brief Функция увеличивает максимальное здоровье, сохраняя соотношение текущего здоровья к максимальному
	* @param int amount - количество добавляемого здоровья
	*/
	void IncreaseHealth(int amount)
	{
		float healthRatio = static_cast<float>(currentHealth) / maxHealth;
		maxHealth += amount;
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
	*/
	PlayerHealthComponent(int maxHealth) : HealthComponent(maxHealth) {}
};

/**
* @brief Компонент хранит данные, необходимые для получения урона по здоровью игрока
*/
struct BossHealthComponent : public HealthComponent
{
	/**
	* @brief Основной конструктов
	* @param int maxHealth - максимальное здоровье сущности
	*/
	BossHealthComponent(int maxHealth) : HealthComponent(maxHealth) {}
};

/**
* @brief Компонент показывает, сколько урона наносит сущность
*/
struct DamageComponent : public Component
{
	/**
	* @brief Основной конструктор
	* @param int amount - количество наносимого урона
	* @param float cooldown - время перезарядки между атаками
	* @param EntityType targetType - тип сущности, которой будет наноситься урон
	*/
	DamageComponent(int amount, float cooldown, EntityType targetType)
		: amount(amount), cooldown(cooldown), timer(0.f), targetType(targetType) {
	}

	int amount;
	float timer;
	float cooldown;
	EntityType targetType;

	/**
	* @brief Функция обновляет таймер перезарядки
	* @param float dt - время на которое уменьшается таймер
	*/
	void UpdateCooldown(float dt)
	{
		timer -= dt;
	}

	/**
	* @brief Функция проверяет, готово ли оружие для нанесения урона
	* @return true, если можно наносить урон, иначе false
	*/
	bool CanDealDamage() const
	{
		return timer <= 0.f;
	}

	/**
	* @brief Функция наносит урон компоненту здоровья
	* @param HealthComponent* targetHealth - компонент здоровья атакуемой сущности
	*/
	void DealDamage(HealthComponent* targetHealth) const
	{
		if (targetHealth == nullptr || !CanDealDamage())
		{
			return;
		}

		targetHealth->RemoveHealth(amount);
	}

	/**
	* @brief Начинает перезарядку после нанесения урона
	*/
	void StartCooldown()
	{
		timer = cooldown;
	}
};

/**
* @brief Компонент хранит данные, необходимые для получения опыта сущности
*/
struct LevelComponent : public Component
{
	/**
	* @brief Основной конструктов
	* @param int maxExperience - максимальное количество опыта на первом уровне
	*/
	LevelComponent(int maxExperience) : maxExperience(maxExperience) {}

	const float XP_PER_LVL_MULT = 1.5f;

	int maxExperience = 0;
	int currentExperience = 0;
	int level = 1;
	bool levelUpFlag = false;

	/**
	* @brief Функция добавления опыта. При достаточном количестве опыта уровень повысится автоматически
	* @param int xpAmount - количество полученного опыта
	*/
	void GainExperience(int amount)
	{
		currentExperience += amount;
		TryLevelUp();
	}

	/**
	* @brief Функция условного повышения уровня
	*/
	bool TryLevelUp()
	{
		if (currentExperience >= maxExperience)
		{
			LevelUp();
			return true;
		}
		return false;
	}

	/**
	* @brief Функция принудительного повышения уровня
	*/
	void LevelUp()
	{
		currentExperience = 0;
		maxExperience = static_cast<int>(maxExperience * XP_PER_LVL_MULT);
		level++;
		levelUpFlag = true;
	}
};

/**
* @brief Компонент обозначает, что за сущностью будет оставаться след из проджектайлов
*/
struct TrailComponent : public Component
{
	/**
	* @brief Основной конструктор
	* @param int damage - количество урона проджектайла следа
	* @param float interval - задержка перед созданием нового проджектайла следа
	*/
	TrailComponent(int damage, float interval) : damage(damage), spawnInterval(interval) {}

	int damage;
	float spawnInterval;
	float elapsedTime = 0.0f;
};

/**
* @brief Компонент обозначает что сущность является бонусом
*/
struct BonusComponent : public Component
{
	enum BonusType {
		Health,
		Bomb,
		Magnet,
	};

	BonusType type;

	/**
	* @brief Основной конструктор
	* @BonusType type - тип бонуса
	*/
	BonusComponent(BonusType type) : type(type) {}
};

/**
* @brief Компонент обозначает что сущность является контейнером
*/
struct ContainerComponent : public Component
{
	/**
	* @brief Основной конструктор
	*/
	ContainerComponent() {}

	bool isDestroyed = false;
};

/**
* @brief Компонент обозначает что сущность является опытом
*/
struct ExperienceComponent : public Component
{
	/**
	* @brief Основной конструктор
	*/
	ExperienceComponent(int amount) : amount(amount) {}

	int amount;
};

/**
* @brief Компонент обозначает что сущность проигрывает последнюю анимацию смерти
*/
struct DeathAnimationComponent : public Component
{
	bool isCompleted = false;
	float elapsedTime = 0.f;
	float currentZoom = 1.f;
};

/**
* @brief Наличие компонента в игре означает поражение
*/
struct GameOverComponent : public Component {};

struct VictoryComponent : public Component {};