#include "SFML/Graphics.hpp"

struct Component
{
	virtual ~Component() = default;
};

struct InputComponent : public Component {};

struct CameraComponent : public Component {};

struct PositionComponent : public Component
{
	float x, y;
	PositionComponent(float x = 0, float y = 0) : x(x), y(y) {}
};

struct VelocityComponent : public Component
{
	float vx, vy;
	VelocityComponent(float vx = 0, float vy = 0) : vx(vx), vy(vy) {}
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