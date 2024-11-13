#pragma once

#include "System.h"

class ProjectileSystem : public System
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	bool IsOutOfBound(float x, float y);
};

