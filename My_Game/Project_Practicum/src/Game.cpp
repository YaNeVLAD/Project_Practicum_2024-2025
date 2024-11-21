#include "Game.h"

#include "Factory/PlayerFactory/PlayerFactory.h"
#include "Factory/EnemyFactory/EnemyFactory.h"
#include "Factory/SystemFactory/SystemFactory.h"
#include "Entity/Weapon/Weapon.h"
#include <random>

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
	auto player = mEntityManager.GetEntitiesWithComponents<WeaponComponent>();

	if (player.size())
	{
		auto weaponComponent = player.front()->GetComponent<WeaponComponent>();
		std::vector<std::string> weaponNames;

		for (auto& weapon : weaponComponent->weapons)
		{
			weaponNames.push_back(weapon->GetName());
		}

		if (weaponNames.size() <= 3)
		{
			return weaponNames;
		}

		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(weaponNames.begin(), weaponNames.end(), g);

		return weaponNames;
	}

	return {};
}

void Game::UpgradeWeapon(size_t index)
{
	auto player = mEntityManager.GetEntitiesWithComponents<WeaponComponent, ExperienceComponent>();
	if (player.size())
	{
		auto weaponComponent = player.front()->GetComponent<WeaponComponent>();
		auto experience = player.front()->GetComponent<ExperienceComponent>();

		if (index < weaponComponent->weapons.size())
		{
			weaponComponent->weapons[index]->Upgrade(experience->level);
		}
		else
		{
		}
	}
}
