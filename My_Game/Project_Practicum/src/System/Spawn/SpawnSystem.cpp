#include "SpawnSystem.h"
#include "../../Factory/EnemyFactory/EnemyFactory.h"
#include <iostream>

void SpawnSystem::Update(EntityManager& entityManager, float deltaTime)
{
	mTimeSinceLastSpawn += deltaTime;

	if (mTimeSinceLastSpawn >= mSpawnInterval)
	{
		sf::Vector2f position = SelectSpawnPosition();
		EnemyFactory::Create(entityManager, position.x, position.y);
		mTimeSinceLastSpawn = 0.0f;
	}
}

sf::Vector2f SpawnSystem::SelectSpawnPosition()
{
    // Получаем размеры окна и параметры камеры
    sf::FloatRect cameraBounds = mCamera.getViewport();
    sf::Vector2f cameraCenter = mCamera.getCenter();
    sf::Vector2f cameraSize = mCamera.getSize();

    if (cameraSize.x == 0.0f || cameraSize.y == 0.0f)
    {
        std::cerr << "Error: Camera size is zero!" << std::endl;
        return sf::Vector2f(0, 0);
    }

    // Размеры экрана
    float screenWidth = cameraSize.x;
    float screenHeight = cameraSize.y;

    int side = rand() % 4;

    float spawnX = 0.0f, spawnY = 0.0f;

    switch (side)
    {
    case 0:
        spawnX = rand() % static_cast<int>(screenWidth + cameraCenter.x - screenWidth / 2.0f);
        spawnY = cameraCenter.y - screenHeight / 2.0f - 40;
        break;
    case 1:
        spawnX = rand() % static_cast<int>(screenWidth + cameraCenter.x - screenWidth / 2.0f);
        spawnY = cameraCenter.y + screenHeight / 2.0f + 40;
        break;
    case 2:
        spawnX = cameraCenter.x - screenWidth / 2.0f - 40;
        spawnY = rand() % static_cast<int>(screenHeight + cameraCenter.y - screenHeight / 2.0f);
        break;
    case 3:
        spawnX = cameraCenter.x + screenWidth / 2.0f + 40;
        spawnY = rand() % static_cast<int>(screenHeight + cameraCenter.y - screenHeight / 2.0f);
        break;
    }

    return sf::Vector2f(spawnX, spawnY);
}
