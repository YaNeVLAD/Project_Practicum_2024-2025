#pragma once

#include "../System.h"

class DamageSystem : public IUpdateSystem
{
	void Update(EntityManager& entityManager, float deltaTime) override;
};