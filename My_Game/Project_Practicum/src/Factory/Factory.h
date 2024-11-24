#pragma once

#include "SFML/Graphics.hpp"
#include "../Entity/Weapon/Weapon.h"

class EntityManager;
class SystemManager;

class Factory
{
public:
	static void InitSystems(SystemManager& systemManager, sf::RenderWindow& window, sf::View& camera, bool& isBossSpawned);
	static std::unique_ptr<Weapon> CreateWeapon(const std::string& weaponName);
	static void CreateEnemy(EntityManager& entityManager, float x, float y);
	static void CreateBoss(EntityManager& entityManager, float x, float y);
	static void CreatePlayer(EntityManager& entityManager);
};