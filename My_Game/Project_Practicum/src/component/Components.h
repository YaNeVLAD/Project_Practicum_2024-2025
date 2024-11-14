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
	float x, y;
	float vx, vy;
	sf::Vector2f lastDirection = { 1.0f, 0.0f };

	/**
	* @brief Основной конструктор
	* @param float x - координата
	* @param float y - координата
	* @param float vx - скорость по оси x
	* @param float vy - скорость по оси y
	*/
	TransformComponent(float x = 0, float y = 0, float vx = 0, float vy = 0) : x(x), y(y), vx(vx), vy(vy) {}
};

/**
* @brief Компонент отвечает за вид объекта на сцене
* @brief Может быть создан без текстуры, в таком случае необходимо задать размер и цвет прямоугольника
*/
struct DrawableComponent : public Component
{
	sf::Sprite sprite;
	sf::Texture texture;

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
		else
		{
			std::cerr << "Не удалось создать текстуру" << std::endl;
		}
	}

	/**
	* @brief Конструктор для текстуры
	* @param const sf::Texture& texture - опциональный, компонент будет отображать переданную текстуру
	* @param sf::Vector2f scale - масштаб объекта
	*/
	DrawableComponent(const sf::Texture& texture, sf::Vector2f scale) : texture(texture)
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
*/
struct ProjectileComponent : public Component
{
	float travelledDistance = 0.0f;
	float maxDistance = 0.0f;

	/**
	* @brief Основной конструктор
	* @param float maxDistance - максимальная дистанция после которой снаряд будет удалён
	* @param float speed - фиксированная скорость снаряда
	*/
	ProjectileComponent(float maxDistance) : maxDistance(maxDistance) {}
};

/**
* @brief Компонент хранит данные, необходимые для анимации спрайта
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
		) : frames(std::move(frames)), frameTime(frameTime), loop(loop), duration(duration), isAnimating(isAnimating) {}
};