#pragma once

#include "../System.h"

class SpawnSystem : public IUpdateSystem
{
public:
	SpawnSystem(sf::View& camera, float spawnInterval) : mCamera(camera), mSpawnInterval(spawnInterval) {}

	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	sf::View& mCamera;
	float mSpawnInterval;
	float mTimeSinceLastSpawn = 0.0f;
	
	const int MAX_ENTITES_ON_SCREEN = 30;

	sf::Vector2f SelectSpawnPosition();
};

