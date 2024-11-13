#pragma once

#include "SFML/Graphics.hpp"

class Weapon;

struct Component
{
	virtual ~Component() = default;
};

struct InputComponent : public Component {};

struct CameraComponent : public Component {};

struct TransformComponent : public Component
{
	float x = 0, y = 0;
	float vx = 0, vy = 0;
	TransformComponent(float x, float y, float vx, float vy) : x(x), y(y), vx(vx), vy(vy) {}
};

struct DrawableComponent : public Component
{
	sf::CircleShape shape;
	DrawableComponent(float radius, sf::Color color)
	{
		shape.setRadius(radius);
		shape.setFillColor(color);
	}
};

struct WeaponComponent : public Component
{
	std::vector<std::unique_ptr<Weapon>> weapons;

	void AddWeapon(std::unique_ptr<Weapon> weapon);
};

struct ProjectileComponent : public Component 
{
	float startX = 0, startY = 0;
};