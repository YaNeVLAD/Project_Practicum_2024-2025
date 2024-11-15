#pragma once

#include "../System.h"

class OrbitalProjectileSystem : public IUpdateSystem
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;
};

