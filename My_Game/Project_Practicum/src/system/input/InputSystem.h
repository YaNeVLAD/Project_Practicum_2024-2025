#pragma once

#include "../System.h"

class InputSystem : public System
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;
};