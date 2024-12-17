#include "SpawnSystem.h"

#include <iostream>

#include "../../Factory/Factory.h"

void SpawnSystem::Update(EntityManager& entityManager, float deltaTime)
{
	mTimeSinceLastEnemySpawn += deltaTime;
	mTimeSinceLastBonusSpawn += deltaTime;
	mTimeSinceLastBossSpawn += deltaTime;

	if (mTimeSinceLastBossSpawn >= mBossSpawnInterval && mSpawnedBosses < mMaxBosses)
	{
		SpawnBoss(entityManager);
		mTimeSinceLastBossSpawn = 0.f;
	}

	auto enemies = entityManager.GetEntitiesWithType(Enemy);
	auto containers = entityManager.GetEntitiesWithComponents<ContainerComponent>();

	if (mTimeSinceLastEnemySpawn >= mEnemySpawnInterval && enemies.size() < MAX_ENEMIES_ON_SCREEN)
	{
		sf::Vector2f position = SelectSpawnPosition();
		Factory::CreateEnemy(entityManager, position);
		mTimeSinceLastEnemySpawn = 0.f;
	}

	if (mTimeSinceLastBonusSpawn >= mBonusSpawnInterval && containers.size() < MAX_CONTAINERS_ON_SCREEN)
	{
		sf::Vector2f position = SelectSpawnPosition();
		Factory::CreateContainer(entityManager, position);
		mTimeSinceLastBonusSpawn = 0.f;
	}
}

sf::FloatRect SpawnSystem::getCameraBounds()
{
	sf::Vector2f cameraCenter = mCamera.getCenter();
	sf::Vector2f cameraSize = mCamera.getSize();

	if (cameraSize.x == 0.0f || cameraSize.y == 0.0f)
	{
		return sf::FloatRect(0.f, 0.f, 0.f, 0.f);
	}

	float left = cameraCenter.x - cameraSize.x / 2.0f;
	float top = cameraCenter.y - cameraSize.y / 2.0f;
	float right = cameraCenter.x + cameraSize.x / 2.0f;
	float bottom = cameraCenter.y + cameraSize.y / 2.0f;

	return sf::FloatRect(left, top, right, bottom);
}

sf::Vector2f SpawnSystem::SelectSpawnPosition()
{
	sf::FloatRect bounds = getCameraBounds();

	std::uniform_real_distribution<float> horizontalDistribution(bounds.left, bounds.width);
	std::uniform_real_distribution<float> verticalDistribution(bounds.top, bounds.height);

	float horizontalDistance = horizontalDistribution(mGenerator);
	float verticalDistance = verticalDistribution(mGenerator);

	sf::Vector2f distance = { horizontalDistance, verticalDistance };

	return generatePosition(distance);
}

sf::Vector2f SpawnSystem::generatePosition(sf::Vector2f& distance)
{
	sf::FloatRect bounds = getCameraBounds();

	std::uniform_int_distribution<int> sideDistribution(0, 3);

	int side = sideDistribution(mGenerator);
	sf::Vector2f spawn = { 0.f, 0.f };

	switch (side)
	{
	case 0:
		spawn.x = distance.x;
		spawn.y = bounds.top - 40;
		break;
	case 1:
		spawn.x = distance.x;
		spawn.y = bounds.height + 40;
		break;
	case 2:
		spawn.x = bounds.left - 40;
		spawn.y = distance.y;
		break;
	case 3:
		spawn.x = bounds.width + 40;
		spawn.y = distance.y;
		break;
	}

	return spawn;
}

void SpawnSystem::SpawnBoss(EntityManager& em)
{
	sf::Vector2f pos = SelectSpawnPosition();
	Factory::CreateBoss(em, pos);
	mSpawnedBosses++;
}