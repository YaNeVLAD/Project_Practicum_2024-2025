#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include <optional>

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
* @brief Компонент хранит данные о положении объекта на сцене
* @param x - координата
* @param y - координата
* @param vx - скорость по оси x
* @param vy - скорость по оси y
*/
struct TransformComponent : public Component
{
	float x, y;
	float vx, vy;
	TransformComponent(float x = 0, float y = 0, float vx = 0, float vy = 0) : x(x), y(y), vx(vx), vy(vy) {}
};

/**
* @brief Компонент отвечает за вид объекта на сцене
* @brief Может быть создан без текстуры, в таком случае необходимо задать размер и цвет прямоугольника
* @param width - ширина прямоугольника
* @param height - высота прямоугольника
* @param color - цвет прямоугольника
* @param texture - опциональный, компонент будет отображать переданную текстуру
* @param scale - масштаб объекта
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
            std::cerr << "Не удалось создать текстуру" << std::endl;
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
* @brief Компонент хранит переданное оружие
*/
struct WeaponComponent : public Component
{
	std::vector<std::unique_ptr<Weapon>> weapons;

	void AddWeapon(std::unique_ptr<Weapon> weapon);
};

/**
* @brief Компонент обозначает что сущность является снарядом
* @param maxDistance - максимальная дистанция после которой снаряд будет удалён
*/
struct ProjectileComponent : public Component 
{
	float travelledDistance = 0.0f;
	float maxDistance = 0.0f;
	ProjectileComponent(float maxDistance) : maxDistance(maxDistance) {}
};

/**
* @brief Компонент хранит данные, необходимые для анимации спрайта
* @param frames - набор картинок для покадровой анимации
* @param frameTime - время отображения каждого кадра
* @param loop - флаг, указывает на необходимость зациклить анимацию
* @param duration - время всей анимации
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