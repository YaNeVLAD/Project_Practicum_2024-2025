#pragma once

#include "System.h"

class MovementSystem : public System
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;
};