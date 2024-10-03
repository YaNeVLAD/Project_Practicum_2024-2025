#include "Player.h"

Player::Player(float _x, float _y)
{
	x = _x;
	y = _y;

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

void Player::setSpeed(float _speed)
{
	speed = _speed;
}

float Player::getRadius()
{
	return radius;
}

float Player::getX()
{
	return x;
}

float Player::getY()
{
	return y;
}

void Player::setX(float _x)
{
	x = _x;
}

void Player::setY(float _y)
{
	y = _y;
}

void Player::setPosition(Vector2f position)
{
	shape.setPosition(position);
}

Vector2f Player::getPosition()
{
	return shape.getPosition();
}

void Player::draw(RenderWindow& window)
{
	window.draw(shape);
}