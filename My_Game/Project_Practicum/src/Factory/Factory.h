#pragma once

#include "SFML/Graphics.hpp"
#include "../Entity/Weapon/Weapon.h"

class EntityManager;
class SystemManager;

class Factory
{
public:
	static void InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera, bool& isBossSpawned, bool& isPaused);
	static std::unique_ptr<Weapon> CreateWeapon(const std::string& weaponName);
	static void CreateEnemy(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateBoss(EntityManager& entityManager, sf::Vector2f pos);
	static void CreatePlayer(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateHealthBonus(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateBombBonus(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateMagnetBonus(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateContainer(EntityManager& entityManager, sf::Vector2f pos);
	static void CreateExperience(EntityManager& entityManager, sf::Vector2f pos);
};