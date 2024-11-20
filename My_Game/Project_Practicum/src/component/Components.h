#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include <optional>

enum Direction
{
	Left = -1,
	Right = 1,
	Top = 2,
	Bottom = 3,
};

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
* @brief ��������� ���������� ��� �������� �������� ��������
*/
struct ProjectileComponent : public Component
{
	float travelledDistance = 0.0f;
	float maxDistance = 0.0f;

	/**
	* @brief �������� �����������
	* @param float maxDistance - ������������ ��������� ����� ������� ������ ����� �����
	* @param float speed - ������������� �������� �������
	*/
	ProjectileComponent(float maxDistance) : maxDistance(maxDistance) {}
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
		) : frames(std::move(frames)), frameTime(frameTime), loop(loop), duration(duration), isAnimating(isAnimating) {}
};