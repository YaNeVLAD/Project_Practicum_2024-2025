#pragma once

#include "../System.h"

#include <random>

class SpawnSystem : public IUpdateSystem
{
public:
	SpawnSystem(sf::View& camera, float enemySpawnInterval, float bonusSpawnInterval, float bossSpawnInterval, size_t* maxBosses)
		: mCamera(camera),
		mEnemySpawnInterval(enemySpawnInterval),
		mBonusSpawnInterval(bonusSpawnInterval),
		mBossSpawnInterval(bossSpawnInterval),
		mMaxBosses(*maxBosses),
		mGenerator(mRd()) {}

	void Update(EntityManager& entityManager, float deltaTime) override;
private:
	std::random_device mRd;
	std::mt19937 mGenerator;

	sf::View& mCamera;
	float mEnemySpawnInterval;
	float mBonusSpawnInterval;
	float mBossSpawnInterval;

	size_t mMaxBosses;
	size_t mSpawnedBosses = 0;

	float mTimeSinceLastEnemySpawn = 0.f;
	float mTimeSinceLastBonusSpawn = 0.f;
	float mTimeSinceLastBossSpawn = 0.f;

	float mElapsedTime = 0.0f;

	const int MAX_ENEMIES_ON_SCREEN = 100;
	const int MAX_CONTAINERS_ON_SCREEN = 10;

	sf::FloatRect getCameraBounds();

	sf::Vector2f generatePosition(sf::Vector2f& distance);
	sf::Vector2f SelectSpawnPosition();

	void SpawnBoss(EntityManager& em);
};