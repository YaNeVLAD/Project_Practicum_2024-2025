#pragma once

#include "../System.h"

class SpawnSystem : public System
{
public:
	SpawnSystem(sf::View& camera, float spawnInterval) : mCamera(camera), mSpawnInterval(spawnInterval) {}

	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	sf::View& mCamera;
	float mSpawnInterval;
	float mTimeSinceLastSpawn = 0.0f;

	sf::Vector2f SelectSpawnPosition();
};

