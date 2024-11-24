#pragma once

#include "../System.h"

class SpawnSystem : public IUpdateSystem
{
public:
	SpawnSystem(sf::View& camera, float spawnInterval, bool& isBossSpawned) 
		: mCamera(camera), mSpawnInterval(spawnInterval), mIsBossSpawned(isBossSpawned) {}

	void Update(EntityManager& entityManager, float deltaTime) override;
private:
	sf::View& mCamera;
	float mSpawnInterval;
	float mTimeSinceLastSpawn = 0.0f;

	float mElapsedTime = 0.0f;
	const float BOSS_SPAWN_TIME = 5.0f;
	bool& mIsBossSpawned;
	
	const int MAX_ENTITES_ON_SCREEN = 30;

	sf::Vector2f SelectSpawnPosition();
	void SpawnBoss(EntityManager& em);
};