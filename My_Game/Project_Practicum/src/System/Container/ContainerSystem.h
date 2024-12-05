#pragma once

#include "../System.h"

#include <random>

class ContainerSystem : public IUpdateSystem
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	std::random_device mRd;
	std::mt19937 mGenerator;

	void SpawnBonus(EntityManager& em, sf::Vector2f position);
};