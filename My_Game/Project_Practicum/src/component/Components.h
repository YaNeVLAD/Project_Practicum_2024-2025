#pragma once

#include "SFML/Graphics.hpp"

class Entity;
enum EntityType;
class Weapon;

/**
* @brief ����� ���������-����������� ��� ����� �����������
*/
struct Component
{
	virtual ~Component() = default;
};

/**
* @brief ��������� �������� �� ��������� ������ ��� ������� ������
*/
struct InputComponent : public Component {};

/**
* @brief ��������� �������� �� ��������� ������
*/
struct CameraComponent : public Component {};

/**
* @brief ��������� ������ ��������� �������� � ������������
*/
struct TransformComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param sf::Vector2f position - ������ �������
	* @param sf::Vector2f velocity - ������ ��������
	*/
	TransformComponent(sf::Vector2f position = { 0, 0 }, sf::Vector2f velocity = { 0, 0 })
		: x(position.x), y(position.y), vx(velocity.x), vy(velocity.y) {
	}

	sf::Vector2f GetPosition()
	{
		return { x, y };
	}

	sf::Vector2f GetVelocity()
	{
		return { vx, vy };
	}

	float x, y;
	float vx, vy;
	sf::Vector2f lastDirection = { 0.f, 0.f };
};

/**
* @brief ��������� ������ ���� ������� ��������
*/

struct RotationComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param float angle - ���� ������� ��������
	*/
	RotationComponent(float angle = 0.0f) : angle(angle) {}

	float angle;
};

/**
* @brief ��������� ������ ������������ ������ ��������
*/
struct CollisionComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param std::unique_ptr<sf::Shape> shape - ��������� �� ������ ��������
	* @param sf::Vector2f offset - �������� ������������ ������ ��������
	* @param float cooldown - ����������� ��������
	*/
	CollisionComponent(std::unique_ptr<sf::RectangleShape> shape, sf::Vector2f offset = sf::Vector2f(0.0f, 0.0f), float cooldown = 0.0f)
		: shape(std::move(shape)), offset(offset), cooldown(cooldown) {
	}

	std::unique_ptr<sf::RectangleShape> shape;
	sf::Vector2f offset;
	float elapsedTime = 0.0f;
	float cooldown;

	/**
	* @brief ����� ��� ��������� �������� ���� ��������
	*/
	sf::FloatRect getRect() const
	{
		return shape->getGlobalBounds();
	}

	/**
	* @brief ����� ��� ���������� ������� ���� ��������
	* @param float x - ���������� x
	* @param float y - ���������� y
	*/
	void UpdatePosition(float x, float y) const
	{
		shape->setPosition(x + offset.x, y + offset.y);
	}
};

/**
* @brief ��������� �������� �� ��� ������� �� �����
* @brief ����� ���� ������ ��� ��������, � ����� ������ ���������� ������ ������ � ���� ��������������
*/
struct DrawableComponent : public Component
{
	/**
	* @brief ����������� ��� ��������
	* @param const sf::Texture& texture - ������������, ��������� ����� ���������� ���������� ��������
	* @param sf::Vector2f scale - ������� �������
	*/
	DrawableComponent(const sf::Texture& texture, sf::Vector2f scale = { 1, 1 }) : texture(texture)
	{
		sprite.setTexture(texture);
		sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
		sprite.setScale(scale.x, scale.y);
	}

	/**
	* @brief ����������� ��� �������� ��������������
	* @param int width - ������ ��������������
	* @param int height - ������ ��������������
	* @param sf::Color color - ���� ��������������
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
* @brief ��������� ������ ���������� ������
*/
struct WeaponComponent : public Component
{
	/**
	* @brief ������� ��������� ������ � ������ ������ ��������
	*/
	void AddWeapon(std::unique_ptr<Weapon> weapon);
	std::vector<std::unique_ptr<Weapon>> weapons;
};

/**
* @brief ��������� ���������� ��� �������� ����� ����� ����� � ����� ������� �� ��� ���������
*/
struct LifetimeComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param float maxLifetime - ������������ ����� �����, �� ��������� �������� �������� ����� �������
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
* @brief ��������� ���������� ��� �������� ����� ��������� �� ������ ������ ������ ��������
*/
struct OrbitalProjectileComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param float radius - ������ ������
	* @param float speed - ������� ��������
	* @param TransformComponent* parent - ��������� �� TransformComponent ��������
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
* @brief ��������� ������ ������, ����������� ��� �������� �������
*/
struct AnimationComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param std::vector<sf::Texture> frames - ����� �������� ��� ���������� ��������
	* @param float frameTime - ����� ����������� ������� �����
	* @param bool loop - ����, ��������� �� ������������� ��������� ��������
	* @param float duration - ����� ���� ��������
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
* @brief ��������� ������ ������, ����������� ��� ��������� ����� �� ��������
*/
struct HealthComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param int maxHealth - ������������ �������� ��������
	*/
	HealthComponent(int maxHealth) : maxHealth(maxHealth), currentHealth(maxHealth) {}

	int maxHealth;
	int currentHealth;

	void RemoveHealth(int amount)
	{
		currentHealth -= amount;
	}

	void AddHealth(int amount)
	{
		(currentHealth + amount > maxHealth)
			? currentHealth = maxHealth
			: currentHealth += amount;
	}

	bool IsAlive() const
	{
		return currentHealth > 0;
	}

	void IncreaseHealth(int amount)
	{
		float healthRatio = static_cast<float>(currentHealth) / maxHealth;
		maxHealth += amount;
		currentHealth = static_cast<int>(maxHealth * healthRatio);
	}
};

/**
* @brief ��������� ������ ������, ����������� ��� ��������� ����� �� �������� �����
*/
struct PlayerHealthComponent : public HealthComponent
{
	/**
	* @brief �������� �����������
	* @param int maxHealth - ������������ �������� ��������
	*/
	PlayerHealthComponent(int maxHealth) : HealthComponent(maxHealth) {}
};

/**
* @brief ��������� ������ ������, ����������� ��� ��������� ����� �� �������� ������
*/
struct BossHealthComponent : public HealthComponent
{
	/**
	* @brief �������� �����������
	* @param int maxHealth - ������������ �������� ��������
	*/
	BossHealthComponent(int maxHealth) : HealthComponent(maxHealth) {}
};

/**
* @brief ��������� ����������, ������� ����� ������� ��������
*/
struct DamageComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param int amount - ���������� ���������� �����
	* @param EntityType targetType - ��� ��������, ������� ����� ���������� ����
	*/
	DamageComponent(int amount, float cooldown, EntityType targetType)
		: amount(amount), cooldown(cooldown), timer(cooldown), targetType(targetType) {
	}

	int amount;
	float timer;
	float cooldown;

	HealthComponent* lastTargetHealth = nullptr;

	EntityType targetType;

	void UpdateCooldown(float dt)
	{
		timer -= dt;
	}

	bool CanDealDamage(HealthComponent* targetHealth) const
	{
		return timer <= 0.f;
	}

	void DealDamage(HealthComponent* targetHealth)
	{
		if (targetHealth == nullptr)
		{
			return;
		}

		if (targetHealth != lastTargetHealth)
		{
			lastTargetHealth = targetHealth;
			targetHealth->RemoveHealth(amount);
			timer = cooldown;
		}

		if (CanDealDamage(targetHealth))
		{
			targetHealth->RemoveHealth(amount);
			timer = cooldown;
		}
	}
};

/**
* @brief ��������� ������ ������, ����������� ��� ��������� ����� ��������
*/
struct LevelComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param int maxExperience - ������������ ���������� ����� �� ������ ������
	*/
	LevelComponent(int maxExperience)
		: maxExperience(maxExperience) {
	}

	const float XP_PER_LVL_MULT = 1.5f;

	int maxExperience = 0;
	int currentExperience = 0;
	int level = 1;
	bool levelUpFlag = false;

	/**
	* @brief ������� ���������� �����. ��� ����������� ���������� ����� ������� ��������� �������������
	* @param int xpAmount - ���������� ����������� �����
	*/
	void GainExperience(int amount)
	{
		currentExperience += amount;
		TryLevelUp();
	}

	/**
	* @brief ������� ��������� ��������� ������
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
	* @brief ������� ��������������� ��������� ������
	*/
	void LevelUp()
	{
		currentExperience = 0;
		maxExperience = static_cast<int>(maxExperience * XP_PER_LVL_MULT);
		level++;
		levelUpFlag = true;
	}
};

struct TrailComponent : public Component
{
	TrailComponent(int damage, float interval) : damage(damage), spawnInterval(interval) {}

	int damage;
	float spawnInterval;
	float elapsedTime = 0.0f;
};

struct BonusComponent : public Component
{
	enum BonusType {
		Health,
		Bomb,
	};

	BonusType type;

	BonusComponent(BonusType type) : type(type) {}
};

struct ContainerComponent : public Component
{
	ContainerComponent() {}

	bool isDestroyed = false;
};

struct ExperienceComponent : public Component
{
	ExperienceComponent(int amount) : amount(amount) {}

	int amount;
};