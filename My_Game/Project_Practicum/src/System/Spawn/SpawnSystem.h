#pragma once

#include "../System.h"

#include <random>

class SpawnSystem : public IUpdateSystem
{
public:
	SpawnSystem(sf::View& camera, float enemySpawnInterval, float bonusSpawnInterval, bool& isBossSpawned)
		: mCamera(camera),
		mEnemySpawnInterval(enemySpawnInterval),
		mBonusSpawnInterval(bonusSpawnInterval),
		mIsBossSpawned(isBossSpawned),
		mGenerator(mRd()) {}

	void Update(EntityManager& entityManager, float deltaTime) override;
private:
	std::random_device mRd;
	std::mt19937 mGenerator;

	sf::View& mCamera;
	float mEnemySpawnInterval;
	float mBonusSpawnInterval;

	float mTimeSinceLastEnemySpawn = 0.f;
	float mTimeSinceLastBonusSpawn = 0.f;

	float mElapsedTime = 0.0f;
	const float BOSS_SPAWN_TIME = 300.f;
	bool& mIsBossSpawned;

	const int MAX_ENEMIES_ON_SCREEN = 100;
	const int MAX_CONTAINERS_ON_SCREEN = 10;

	sf::FloatRect getCameraBounds();

	sf::Vector2f generatePosition(sf::Vector2f& distance);
	sf::Vector2f SelectSpawnPosition();

	void SpawnBoss(EntityManager& em);
};