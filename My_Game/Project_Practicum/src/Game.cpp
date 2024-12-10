#include "Game.h"

#include <random>

#include "Entity/Weapon/Weapon.h"
#include "Factory/Factory.h"

void Game::InitSystems()
{
	Factory::InitSystems(mSystemManager, mWindow, mCamera, mIsBossSpawned, mIsPaused);
}

void Game::InitPlayer()
{
	Factory::CreatePlayer(mEntityManager, { mWindow.getSize().x / 2.f, mWindow.getSize().y / 2.f });
}

void Game::InitMap()
{
	mMap.Init("assets/map/Grass.png");
}

void Game::RunFrame(float deltaTime)
{
	mEntityManager.UpdateEntities();

	if (mIsPaused)
	{
		return;
	}

	for (auto& system : mSystemManager.GetUpdateSystems())
	{
		system->Update(mEntityManager, deltaTime);
	}

	mMap.UpdatePosition(mCamera);
}

void Game::PauseGame()
{
	mIsPaused = true;
}

void Game::ResumeGame()
{
	mIsPaused = false;
}

void Game::Reset()
{
	mEntityManager.Clear();
	mSystemManager.Clear();
	mIsPaused = false;
	mIsBossSpawned = false;
}

void Game::Render(float deltaTime)
{
	mWindow.setView(mCamera);

	mWindow.draw(mMap);

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
	return mEntityManager.GetEntitiesWithComponents<GameOverComponent>().size();
}

bool Game::HasPlayerLeveledUp()
{
	auto player = mEntityManager.GetEntitiesWithComponents<LevelComponent>();

	if (player.empty())
	{
		return false;
	}

	auto experience = player.front()->GetComponent<LevelComponent>();
	if (experience->levelUpFlag)
	{
		experience->levelUpFlag = false;
		return true;
	}

	return false;
}

bool Game::IsBossDefeated()
{
	auto boss = mEntityManager.GetEntitiesWithComponents<BossHealthComponent>();

	if (boss.empty())
	{
		return false;
	}

	auto health = boss.front()->GetComponent<BossHealthComponent>();
	return !health->IsAlive();
}

std::vector<std::string> Game::GetAvailableWeapons()
{
	auto allWeapons = Weapon::GetAllWeapons();

	auto player = mEntityManager.GetEntitiesWithComponents<WeaponComponent>();
	if (player.empty())
	{
		return {};
	}

	auto weaponComponent = player.front()->GetComponent<WeaponComponent>();

	std::unordered_map<std::string, bool> playerWeapons;
	for (const auto& weapon : weaponComponent->weapons)
	{
		playerWeapons[weapon->GetName()] = !weapon->CanUpgrade();
	}

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
	auto player = mEntityManager.GetEntitiesWithComponents<WeaponComponent, PlayerHealthComponent>();

	if (player.empty())
	{
		return;
	}

	auto weaponComponent = player.front()->GetComponent<WeaponComponent>();

	for (const auto& weapon : weaponComponent->weapons)
	{
		if (weapon->GetName() == name)
		{
			weapon->Upgrade(1);
			return;
		}
	}

	auto newWeapon = Factory::CreateWeapon(name);
	weaponComponent->AddWeapon(std::move(newWeapon));
}