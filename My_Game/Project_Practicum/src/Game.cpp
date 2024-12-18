#include "Game.h"

#include "Factory/Factory.h"
#include "../ui/Button/Button.h"
#include "Entity/Weapon/Weapon.h"
#include <iostream>

bool Game::CanPause()
{
	return mEntityManager.GetEntitiesWithComponents<DeathAnimationComponent>().empty();
}

void Game::InitKeyBindings()
{
	mScreen.AddKeyBinding(KeyBinding(sf::Keyboard::Escape, [this]()
		{
			if (!CanPause())
			{
				return;
			}

			mIsPaused = !mIsPaused;
			RenderPauseScreen();
		}));
}

void Game::InitSystems()
{
	Factory::InitSystems(mSystemManager, mWindow, mCamera, &mDefeatedBosses, &mMaxBosses, mIsPaused);
}

void Game::InitPlayer()
{
	Factory::CreatePlayer(mEntityManager, { mWindow.getSize().x / 2.f, mWindow.getSize().y / 2.f });
}

void Game::InitMap()
{
	mMap.Init("assets/map/Grass.png");
}

void Game::RenderPauseScreen()
{
	mScreen.Clear();
	if (!mIsPaused)
	{
		return;
	}

	Button continueButton;
	continueButton
		.SetSize({ 200.f, 50.f })
		.SetFillColor(sf::Color::Yellow)
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, 300.f })
		.SetOnClickListener([this]()
			{
				Resume();
				mScreen.Clear();
			});

	mScreen.AddView(std::make_shared<Button>(continueButton));
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

void Game::Pause()
{
	mIsPaused = true;
}

void Game::Resume()
{
	mIsPaused = false;
}

void Game::Reset(size_t bossCount)
{
	mScreen.Clear();
	mScreen.ClearBindings();
	mEntityManager.Clear();
	mSystemManager.Clear();
	mIsPaused = false;
	mMaxBosses = bossCount == -1 ? mMaxBosses : bossCount;
	mMaxBosses = isInfinite ? UINT64_MAX : mMaxBosses;
	mDefeatedBosses = 0;
}

void Game::Render(float deltaTime)
{
	mWindow.setView(mCamera);

	mWindow.draw(mMap);

	for (auto& system : mSystemManager.GetRenderSystems())
	{
		system->Render(mEntityManager, deltaTime);
	}

	mWindow.draw(mScreen);
}

void Game::ProcessEvents(const sf::Event& event)
{
	mScreen.HandleEvents(mWindow, mCamera, event);
}

void Game::ChangeMaxBosses(size_t offset)
{
	if (mMaxBosses + offset >= 1 && mMaxBosses + offset <= 999)
	{
		mMaxBosses += offset;
	}
}

bool Game::IsBossDefeated() const
{
	return mDefeatedBosses >= mMaxBosses;
}

bool Game::IsPlayerDefeated()
{
	return mEntityManager.GetEntitiesWithType(Player).empty();
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

std::vector<std::shared_ptr<Weapon>> Game::GetAvailableWeapons()
{
	auto allWeapons = Weapon::GetAllWeapons();

	auto player = mEntityManager.GetEntitiesWithComponents<WeaponComponent>();
	if (player.empty())
	{
		return {};
	}

	auto weaponComponent = player.front()->GetComponent<WeaponComponent>();

	std::unordered_map<std::string, std::shared_ptr<Weapon>> playerWeapons;
	for (const auto& weapon : weaponComponent->weapons)
	{
		playerWeapons[weapon->GetName()] = weapon;
	}

	std::vector<std::shared_ptr<Weapon>> filteredWeapons;
	for (const auto& weaponName : allWeapons)
	{
		auto it = playerWeapons.find(weaponName);
		if (it == playerWeapons.end())
		{
			auto weapon = Factory::CreateWeapon(weaponName);
			if (weapon)
			{
				filteredWeapons.push_back(weapon);
			}
		}
		else if (it->second->CanUpgrade())
		{
			filteredWeapons.push_back(it->second);
		}
	}

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(filteredWeapons.begin(), filteredWeapons.end(), g);

	if (filteredWeapons.size() > 3)
	{
		filteredWeapons.resize(3);
	}

	return filteredWeapons;
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
	weaponComponent->AddWeapon(newWeapon);
}