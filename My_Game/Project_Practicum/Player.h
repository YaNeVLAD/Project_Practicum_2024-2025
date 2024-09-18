#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;

class Player
{
private:
	float x, y;
	float speed = 10;

	float radius = 50;
	Color color = sf::Color::Green;
	float outlineThickness = 5;
	Color outlineColor = sf::Color::Red;

public:
	CircleShape shape = CircleShape(radius);
	Player(float _x, float _y);
	~Player() {};

	void initShape();
	float getSpeed();

	void draw(RenderWindow& window);
	void move(float x, float y);
};

