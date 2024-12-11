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

	/**
	* @brief ������� ��������� ������� �������
	*/
	sf::Vector2f GetPosition()
	{
		return { x, y };
	}

	/**
	* @brief ������� ��������� ������� ��������
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

/**
* @brief ��������� ���������� ��� �������� ����� ���������� �� ���������� �����
*/
struct HomingProjectileComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param float speed - �������� �������
	* @param EntityType targetType - ��� ��������, �� ������� ����� ���������� ������
	*/
	HomingProjectileComponent(float speed, EntityType targetType)
		: speed(speed), targetType(targetType) {
	}

	float speed;
	EntityType targetType;
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
	enum AnimationState
	{
		IDLE,
		WALK,
		HURT,
		DEAD,
		ATTACK
	};

	/**
	* @brief �������� �����������
	* @param std::vector<sf::Texture> frames - ����� �������� ��� ���������� ��������
	* @param float frameTime - ����� ����������� ������� �����
	* @param bool loop - ����, ��������� �� ������������� ��������� ��������
	* @param float duration - ����� ���� ��������
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

	/**
	* @brief ������� �������� �������� �� ��������
	* @param int amount - ���������� ����������� ��������
	*/
	void RemoveHealth(int amount)
	{
		currentHealth -= amount;
	}

	/**
	* @brief ������� ��������� ��������, �� ���������� ��� ��������
	* @param int amount - ���������� ������������ ��������
	*/
	void AddHealth(int amount)
	{
		(currentHealth + amount > maxHealth)
			? currentHealth = maxHealth
			: currentHealth += amount;
	}

	/**
	* @brief ������� ���������� ���� �������� ��� ���
	*/
	bool IsAlive() const
	{
		return currentHealth > 0;
	}

	/**
	* @brief ������� ����������� ������������ ��������, �������� ����������� �������� �������� � �������������
	* @param int amount - ���������� ������������ ��������
	*/
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
	* @param float cooldown - ����� ����������� ����� �������
	* @param EntityType targetType - ��� ��������, ������� ����� ���������� ����
	*/
	DamageComponent(int amount, float cooldown, EntityType targetType)
		: amount(amount), cooldown(cooldown), timer(0.f), targetType(targetType) {
	}

	int amount;
	float timer;
	float cooldown;
	EntityType targetType;

	/**
	* @brief ������� ��������� ������ �����������
	* @param float dt - ����� �� ������� ����������� ������
	*/
	void UpdateCooldown(float dt)
	{
		timer -= dt;
	}

	/**
	* @brief ������� ���������, ������ �� ������ ��� ��������� �����
	* @return true, ���� ����� �������� ����, ����� false
	*/
	bool CanDealDamage() const
	{
		return timer <= 0.f;
	}

	/**
	* @brief ������� ������� ���� ���������� ��������
	* @param HealthComponent* targetHealth - ��������� �������� ��������� ��������
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
	* @brief �������� ����������� ����� ��������� �����
	*/
	void StartCooldown()
	{
		timer = cooldown;
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
	LevelComponent(int maxExperience) : maxExperience(maxExperience) {}

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

/**
* @brief ��������� ����������, ��� �� ��������� ����� ���������� ���� �� �������������
*/
struct TrailComponent : public Component
{
	/**
	* @brief �������� �����������
	* @param int damage - ���������� ����� ������������ �����
	* @param float interval - �������� ����� ��������� ������ ������������ �����
	*/
	TrailComponent(int damage, float interval) : damage(damage), spawnInterval(interval) {}

	int damage;
	float spawnInterval;
	float elapsedTime = 0.0f;
};

/**
* @brief ��������� ���������� ��� �������� �������� �������
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
	* @brief �������� �����������
	* @BonusType type - ��� ������
	*/
	BonusComponent(BonusType type) : type(type) {}
};

/**
* @brief ��������� ���������� ��� �������� �������� �����������
*/
struct ContainerComponent : public Component
{
	/**
	* @brief �������� �����������
	*/
	ContainerComponent() {}

	bool isDestroyed = false;
};

/**
* @brief ��������� ���������� ��� �������� �������� ������
*/
struct ExperienceComponent : public Component
{
	/**
	* @brief �������� �����������
	*/
	ExperienceComponent(int amount) : amount(amount) {}

	int amount;
};

/**
* @brief ��������� ���������� ��� �������� ����������� ��������� �������� ������
*/
struct DeathAnimationComponent : public Component
{
	bool isCompleted = false;
	float elapsedTime = 0.f;
	float currentZoom = 1.f;
};

/**
* @brief ������� ���������� � ���� �������� ���������
*/
struct GameOverComponent : public Component {};

struct VictoryComponent : public Component {};