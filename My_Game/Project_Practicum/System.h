#pragma once

#include "EntityManager.h"

class System
{
public:
	~System() = default;
	virtual void Update(EntityManager& entityManager, float deltaTime) = 0;
};