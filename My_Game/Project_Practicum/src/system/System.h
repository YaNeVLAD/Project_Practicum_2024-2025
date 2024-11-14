#pragma once

#include "../Manager/Entity/EntityManager.h"
#include "SFML/Graphics.hpp"

class System
{
public:
	~System() = default;
	virtual void Update(EntityManager& entityManager, float deltaTime) = 0;
};