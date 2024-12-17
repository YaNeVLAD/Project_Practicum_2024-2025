#pragma once

#include "../System.h"

class LifetimeSystem : public IUpdateSystem
{
public:
	LifetimeSystem(sf::View& camera, size_t* defeatedBosses) : mCamera(camera), mDefeatedBosses(defeatedBosses) {}

	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	sf::View& mCamera;

	size_t* mDefeatedBosses;

	void TryApplyDeathAnimation(EntityManager& em, Entity* entity, auto* health);
};