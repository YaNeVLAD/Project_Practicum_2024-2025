#pragma once

#include "../System.h"

class OrbitalProjectileSystem : public System
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;
};

