#pragma once

#include "EntityManager.h"
#include "Components.h"

class System
{
public:
	~System() = default;
	virtual void Update(EntityManager& entityManager, float deltaTime) = 0;
};