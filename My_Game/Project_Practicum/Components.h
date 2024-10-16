#pragma once

#include "ECS.h"

class Position : public Component
{
public:
	float x() { return mPos.x; }
	float y() { return mPos.y; }
	sf::Vector2f pos()
	{
		return mPos;
	}

	void init() override
	{
		mPos = { 0, 0 };
	}

	void update() override
	{
		mPos += {1, 1 };
	}

	void setPos(sf::Vector2f pos)
	{
		mPos = pos;
	}
	void setPos(float x, float y)
	{
		mPos = { x, y };
	}

private:
	sf::Vector2f mPos;
};