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
* @brief ��������� ������ ������ � ��������� ������� �� �����
* @param x - ����������
* @param y - ����������
* @param vx - �������� �� ��� x
* @param vy - �������� �� ��� y
*/
struct TransformComponent : public Component
{
	float x, y;
	float vx, vy;
	TransformComponent(float x = 0, float y = 0, float vx = 0, float vy = 0) : x(x), y(y), vx(vx), vy(vy) {}
};

/**
* @brief ��������� �������� �� ��� ������� �� �����
* @brief ����� ���� ������ ��� ��������, � ����� ������ ���������� ������ ������ � ���� ��������������
* @param width - ������ ��������������
* @param height - ������ ��������������
* @param color - ���� ��������������
* @param texture - ������������, ��������� ����� ���������� ���������� ��������
* @param scale - ������� �������
*/
struct DrawableComponent : public Component
{
    sf::Sprite sprite;
    std::optional<sf::Texture> texture;

    DrawableComponent(int width = 128, int height = 128, sf::Color color = sf::Color::White)
    {
        texture.emplace();

        if (texture->create(width, height))
        {
            sf::Image image;
            image.create(width, height, color);
            texture->loadFromImage(image);
            sprite.setTexture(*texture);
            sprite.setOrigin(width / 2.0f, height / 2.0f);
        }
        else
        {
            std::cerr << "�� ������� ������� ��������" << std::endl;
            texture.reset();
        }
    }

    DrawableComponent(const sf::Texture& texture, sf::Vector2f scale)
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
* @param maxDistance - ������������ ��������� ����� ������� ������ ����� �����
*/
struct ProjectileComponent : public Component 
{
	float travelledDistance = 0.0f;
	float maxDistance = 0.0f;
	ProjectileComponent(float maxDistance) : maxDistance(maxDistance) {}
};

/**
* @brief ��������� ������ ������, ����������� ��� �������� �������
* @param frames - ����� �������� ��� ���������� ��������
* @param frameTime - ����� ����������� ������� �����
* @param loop - ����, ��������� �� ������������� ��������� ��������
* @param duration - ����� ���� ��������
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
	AnimationComponent(
		std::vector<sf::Texture> frames, float frameTime, bool loop = true, float duration = -1.0f
	) : frames(std::move(frames)), frameTime(frameTime), loop(loop), duration(duration) {}
};