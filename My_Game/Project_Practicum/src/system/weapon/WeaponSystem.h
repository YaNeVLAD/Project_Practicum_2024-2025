#pragma once

#include "../System.h"

class WeaponSystem : public IUpdateSystem
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;
};