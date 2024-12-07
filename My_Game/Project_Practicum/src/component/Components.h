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
	* @param float x - ����������
	* @param float y - ����������
	* @param float vx - �������� �� ��� x
	* @param float vy - �������� �� ��� y
	*/
	TransformComponent(float x = 0, float y = 0, float vx = 0, float vy = 0) : x(x), y(y), vx(vx), vy(vy) {}

	float x, y;
	float vx, vy;
	sf::Vector2f lastDirection = { 1.0f, 0.0f };
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
	* @param float damageCooldown - ����������� ��� ��������� �����
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
* @brief ��������� ������ ������, ����������� ��� ��������� ����� �� �������� �����
*/
struct PlayerHealthComponent : public HealthComponent
{
	/**
	* @brief �������� �����������
	* @param int maxHealth - ������������ �������� ��������
	* @param float damageCooldown - ����������� ��� ��������� �����
	*/
	PlayerHealthComponent(int maxHealth, float damageCooldown)
		: HealthComponent(maxHealth, damageCooldown) {
	}
};

/**
* @brief ��������� ������ ������, ����������� ��� ��������� ����� �� �������� ������
*/
struct BossHealthComponent : public HealthComponent
{
	/**
	* @brief �������� �����������
	* @param int maxHealth - ������������ �������� ��������
	* @param float damageCooldown - ����������� ��� ��������� �����
	*/
	BossHealthComponent(int maxHealth, float damageCooldown)
		: HealthComponent(maxHealth, damageCooldown) {
	}
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
	DamageComponent(int amount, EntityType targetType) : amount(amount), targetType(targetType) {}

	int amount = 0;

	EntityType targetType;
};

/**
* @brief ��������� ������ ������, ����������� ��� ��������� ����� ��������
*/
struct ExperienceComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param int maxExperience - ������������ ���������� ����� �� ������ ������
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
	* @brief ������� ���������� �����
	* @param int xpAmount - ���������� ����������� �����
	*/
	void GainExperience(int xpAmount)
	{
		currentExperience += xpAmount;
	}
	
	/**
	* @brief ������� ��������� ������
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