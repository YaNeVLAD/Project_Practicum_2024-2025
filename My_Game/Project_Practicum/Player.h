#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;

class Player
{
private:
	float x, y;
	float speed = 15;

	float radius = 50;
	Color color = Color::Green;
	float outlineThickness = 5;
	Color outlineColor = Color::Red;

	CircleShape shape = CircleShape(radius);

	
public:
	Player(float _x, float _y);
	~Player() {};

	void initShape();
	float getSpeed();

	void draw(RenderWindow& window);
	void move(float x, float y);
};

