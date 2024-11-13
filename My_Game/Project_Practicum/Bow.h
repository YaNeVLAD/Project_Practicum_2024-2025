#pragma once

#include "Weapon.h"

class Bow : public Weapon
{
public:
	Bow() { fireRate = 2.0f; }
	void Attack(Entity& projectile, const TransformComponent& startPosition) override;
};

