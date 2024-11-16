#pragma once

#include "../System.h"

class HomingProjectileSystem : public IUpdateSystem
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	Entity* FindClosestTarget(TransformComponent* projectile, const std::vector<Entity*>& enemies);
};

