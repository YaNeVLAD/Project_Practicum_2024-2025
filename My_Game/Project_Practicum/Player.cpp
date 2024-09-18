#include "Player.h"

Player::Player(float _x, float _y)
{
	x = _x;
	y = _y;
}

void Player::initShape()
{
	shape.setPosition(x, y);
	shape.setOrigin(radius / 2, radius / 2);
	shape.setFillColor(color);
	shape.setOutlineColor(outlineColor);
	shape.setOutlineThickness(outlineThickness);
}

float Player::getSpeed()
{
	return speed;
}

void Player::move(float x, float y)
{
	shape.move(x, y);
}

void Player::draw(RenderWindow& window)
{
	window.draw(shape);
}