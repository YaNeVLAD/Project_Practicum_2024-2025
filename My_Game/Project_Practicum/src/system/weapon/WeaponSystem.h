#pragma once

#include "../System.h"

class WeaponSystem : public System
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;
};