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
* @brief Компонент хранит угол наклона сущности
*/

struct RotationComponent : public Component
{
	float angle;

	/**
	* @brief Основной конструктор
	* @param float angle - угол наклона сущности
	*/
	RotationComponent(float angle) : angle(angle) {}
};

/**
* @brief Компонент хранит коллизионную фигуру сущности
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
	* @brief Основной конструктор
	* @param std::unique_ptr<sf::Shape> shape - указатель на фигуру коллизии
	* @param sf::Vector2f offset - смещение относительно центра сущности
	*/
	CollisionComponent(std::unique_ptr<sf::Shape> shape, sf::Vector2f offset = sf::Vector2f(0.0f, 0.0f)) 
		: shape(std::move(shape)), offset(offset) {}
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
* @brief Компонент обозначает что сущность имеет время жизни и будет удалена по его окончанию
*/
struct LifetimeComponent : public Component
{
	float lifetime = 0.0f;
	float maxLifeTime = 0.0f;

	/**
	* @brief Основной конструктор
	* @param float maxLifetime - максимальное время жизни, по истечению которого сущность будет удалена
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
	) : frames(std::move(frames)), frameTime(frameTime), loop(loop), duration(duration), isAnimating(isAnimating) {
	}
};