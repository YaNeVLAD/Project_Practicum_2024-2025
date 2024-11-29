#include "SpawnSystem.h"

#include <random>
#include <iostream>
#include "../../Factory/Factory.h"

void SpawnSystem::Update(EntityManager& entityManager, float deltaTime)
{
	mTimeSinceLastSpawn += deltaTime;

    mElapsedTime += deltaTime;

    if (!mIsBossSpawned && mElapsedTime >= BOSS_SPAWN_TIME)
    {
        SpawnBoss(entityManager);
    }

    auto enemies = entityManager.GetEntitiesWithType(Enemy);
       
	if (mTimeSinceLastSpawn >= mSpawnInterval && enemies.size() < MAX_ENTITES_ON_SCREEN)
	{
		sf::Vector2f position = SelectSpawnPosition();
		Factory::CreateEnemy(entityManager, position.x, position.y);
		mTimeSinceLastSpawn = 0.0f;
	}
}

//Разделить метод
sf::Vector2f SpawnSystem::SelectSpawnPosition()
{
    sf::Vector2f cameraCenter = mCamera.getCenter();
    sf::Vector2f cameraSize = mCamera.getSize();

    if (cameraSize.x == 0.0f || cameraSize.y == 0.0f)
    {
        return sf::Vector2f(0, 0);
    }

    float leftBound = cameraCenter.x - cameraSize.x / 2.0f;
    float rightBound = cameraCenter.x + cameraSize.x / 2.0f;
    float topBound = cameraCenter.y - cameraSize.y / 2.0f;
    float bottomBound = cameraCenter.y + cameraSize.y / 2.0f;

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<int> sideDist(0, 3);
    std::uniform_real_distribution<float> horizontalDist(leftBound, rightBound);
    std::uniform_real_distribution<float> verticalDist(topBound, bottomBound);

    int side = sideDist(generator);
    float spawnX = 0.0f, spawnY = 0.0f;

    switch (side)
    {
    case 0:
        spawnX = horizontalDist(generator);
        spawnY = topBound - 40;
        break;
    case 1:
        spawnX = horizontalDist(generator);
        spawnY = bottomBound + 40;
        break;
    case 2:
        spawnX = leftBound - 40;
        spawnY = verticalDist(generator);
        break;
    case 3:
        spawnX = rightBound + 40;
        spawnY = verticalDist(generator);
        break;
    }

    return sf::Vector2f(spawnX, spawnY);
}

void SpawnSystem::SpawnBoss(EntityManager& em)
{
    sf::Vector2f pos = SelectSpawnPosition();
    Factory::CreateBoss(em, pos.x, pos.y);
    mIsBossSpawned = true;
}