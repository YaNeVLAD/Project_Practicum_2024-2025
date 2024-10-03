#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;

class Player
{
private:
	float x, y;
	float speed = 200;

	float radius = 50;
	Color color = Color::Green;
	float outlineThickness = 5;
	Color outlineColor = Color::Red;

	CircleShape shape = CircleShape(radius);

	
public:
	Player(float _x, float _y);
	~Player() {};

	float getSpeed();
	void setSpeed(float _speed);

	float getRadius();

	float getX();
	float getY();
	
	void setX(float _x);
	void setY(float _y);

	void draw(RenderWindow& window);
	
	void setPosition(Vector2f position);
	Vector2f getPosition();
};

