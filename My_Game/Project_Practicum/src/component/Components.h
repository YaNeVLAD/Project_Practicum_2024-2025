#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include <optional>

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
	float x, y;
	float vx, vy;
	sf::Vector2f lastDirection = { 1.0f, 0.0f };

	/**
	* @brief �������� �����������
	* @param float x - ����������
	* @param float y - ����������
	* @param float vx - �������� �� ��� x
	* @param float vy - �������� �� ��� y
	*/
	TransformComponent(float x = 0, float y = 0, float vx = 0, float vy = 0) : x(x), y(y), vx(vx), vy(vy) {}
};

/**
* @brief ��������� ������ ���� ������� ��������
*/

struct RotationComponent : public Component
{
	float angle;

	/**
	* @brief �������� �����������
	* @param float angle - ���� ������� ��������
	*/
	RotationComponent(float angle) : angle(angle) {}
};

/**
* @brief ��������� ������ ������������ ������ ��������
*/
struct CollisionComponent : public Component
{
	std::unique_ptr<sf::Shape> shape;
	sf::Vector2f offset;

	sf::FloatRect getRect(float x, float y) const
	{
		return shape->getGlobalBounds();
	}

	void UpdatePosition(float x, float y) const
	{
		shape->setPosition(x + offset.x, y + offset.y);
	}

	/**
	* @brief �������� �����������
	* @param std::unique_ptr<sf::Shape> shape - ��������� �� ������ ��������
	* @param sf::Vector2f offset - �������� ������������ ������ ��������
	*/
	CollisionComponent(std::unique_ptr<sf::Shape> shape, sf::Vector2f offset = sf::Vector2f(0.0f, 0.0f)) 
		: shape(std::move(shape)), offset(offset) {}
};

/**
* @brief ��������� �������� �� ��� ������� �� �����
* @brief ����� ���� ������ ��� ��������, � ����� ������ ���������� ������ ������ � ���� ��������������
*/
struct DrawableComponent : public Component
{
	sf::Sprite sprite;
	sf::Texture texture;

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
		else
		{
			std::cerr << "�� ������� ������� ��������" << std::endl;
		}
	}

	/**
	* @brief ����������� ��� ��������
	* @param const sf::Texture& texture - ������������, ��������� ����� ���������� ���������� ��������
	* @param sf::Vector2f scale - ������� �������
	*/
	DrawableComponent(const sf::Texture& texture, sf::Vector2f scale) : texture(texture)
	{
		sprite.setTexture(texture);
		sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
		sprite.setScale(scale.x, scale.y);
	}
};

/**
* @brief ��������� ������ ���������� ������
*/
struct WeaponComponent : public Component
{
	std::vector<std::unique_ptr<Weapon>> weapons;

	void AddWeapon(std::unique_ptr<Weapon> weapon);
};

/**
* @brief ��������� ���������� ��� �������� ����� ����� ����� � ����� ������� �� ��� ���������
*/
struct LifetimeComponent : public Component
{
	float lifetime = 0.0f;
	float maxLifeTime = 0.0f;

	/**
	* @brief �������� �����������
	* @param float maxLifetime - ������������ ����� �����, �� ��������� �������� �������� ����� �������
	*/
	LifetimeComponent(float maxLifeTime) : maxLifeTime(maxLifeTime), lifetime(maxLifeTime) {}
};

struct OrbitalProjectileComponent : public Component
{
	float orbitRadius = 0.0f;
	float orbitSpeed = 0.0f;
	float angle = 0.0f;
	TransformComponent* parentTransform;

	OrbitalProjectileComponent(float radius, float speed, TransformComponent* parent)
		: orbitRadius(radius), orbitSpeed(speed), parentTransform(parent) {
	}
};

/**
* @brief ��������� ������ ������, ����������� ��� �������� �������
*/
struct AnimationComponent : public Component
{
	std::vector<sf::Texture> frames;
	float frameTime = 0.0f;
	float elapsedTime = 0.0f;
	int currentFrameIndex = 0;
	bool loop = true;
	float duration = -1.0f;
	bool isAnimating = false;
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
};