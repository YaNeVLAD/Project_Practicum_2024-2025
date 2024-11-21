#include "Game.h"

#include <random>

#include "Entity/Weapon/Weapon.h"
#include "Factory/PlayerFactory/PlayerFactory.h"
#include "Factory/EnemyFactory/EnemyFactory.h"
#include "Factory/SystemFactory/SystemFactory.h"
#include "Factory/WeaponFactory/WeaponFactory.h"

void Game::InitSystems()
{
	SystemFactory::InitSystems(mSystemManager, mWindow, mCamera);
}

void Game::InitPlayer()
{
	PlayerFactory::Create(mEntityManager);
}

void Game::RunFrame(float deltaTime)
{
	if (mIsPaused)
	{
		return;
	}

	for (auto& system : mSystemManager.GetUpdateSystems())
	{
		system->Update(mEntityManager, deltaTime);
	}
}

void Game::PauseGame()
{
	mIsPaused = true;
}

void Game::ResumeGame()
{
	mIsPaused = false;
}

void Game::Render(float deltaTime)
{
	mWindow.setView(mCamera);
	for (auto& system : mSystemManager.GetRenderSystems())
	{
		system->Render(mEntityManager, deltaTime);
	}
}

void Game::ProcessEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
	}
}

bool Game::IsPlayerDefeated()
{
	auto player = mEntityManager.GetEntitiesWithComponents<PlayerHealthComponent>();
	if (player.size())
	{
		auto health = player.front()->GetComponent<PlayerHealthComponent>();
		return !health->IsAlive();
	}
	return true;
}

bool Game::HasPlayerLeveledUp()
{
	auto player = mEntityManager.GetEntitiesWithComponents<ExperienceComponent>();
	if (player.size())
	{
		auto experience = player.front()->GetComponent<ExperienceComponent>();
		if (experience->levelUpFlag)
		{
			experience->levelUpFlag = false;
			return true;
		}
	}
	return false;
}

std::vector<std::string> Game::GetAvailableWeapons()
{
	auto allWeapons = Weapon::GetAllWeapons();

	auto player = mEntityManager.GetEntitiesWithComponents<WeaponComponent>();
	if (player.empty())
		return {};

	auto weaponComponent = player.front()->GetComponent<WeaponComponent>();

	std::unordered_map<std::string, bool> playerWeapons;
	for (const auto& weapon : weaponComponent->weapons)
	{
		playerWeapons[weapon->GetName()] = !weapon->CanUpgrade();
	}

	// Фильтруем общий список оружий
	allWeapons.erase(
		std::remove_if(
			allWeapons.begin(),
			allWeapons.end(),
			[&](const std::string& weaponName)
			{
				auto it = playerWeapons.find(weaponName);
				if (it != playerWeapons.end())
				{
					return playerWeapons[weaponName];
				}
				return false;
			}),
		allWeapons.end());

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(allWeapons.begin(), allWeapons.end(), g);

	if (allWeapons.size() > 3)
	{
		allWeapons.resize(3);
	}

	return allWeapons;
}

void Game::UpgradeWeapon(std::string name)
{
	auto player = mEntityManager.GetEntitiesWithComponents<WeaponComponent, ExperienceComponent>();
	if (player.size())
	{
		auto weaponComponent = player.front()->GetComponent<WeaponComponent>();
		auto experience = player.front()->GetComponent<ExperienceComponent>();

		for (const auto& weapon : weaponComponent->weapons)
		{
			if (weapon->GetName() == name)
			{
				weapon->Upgrade(1);
				return;
			}
		}

		auto newWeapon = WeaponFactory::Create(name);
		weaponComponent->AddWeapon(std::move(newWeapon));
	}
}