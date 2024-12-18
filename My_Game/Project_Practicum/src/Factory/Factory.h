#pragma once

#include "SFML/Graphics.hpp"
#include "../Entity/Weapon/Weapon.h"

class EntityManager;
class SystemManager;

class Factory
{
public:
	static void InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera, size_t* spawnedBosses, size_t* maxBosses, bool& isPaused);
	static std::shared_ptr<Weapon> CreateWeapon(const std::string& name);
	static void CreateEnemy(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateBoss(EntityManager& entityManager, sf::Vector2f pos);
	static void CreatePlayer(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateHealthBonus(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateBombBonus(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateMagnetBonus(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateContainer(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateExperience(EntityManager& entityManager, sf::Vector2f pos);
	static std::string CreateRandomName();
	static void LoadTextures();
};