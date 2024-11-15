#pragma once

#include "../System.h"

class InputSystem : public IUpdateSystem
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;
};