#include "Game.h"

#include "../ui/Button/Button.h"
#include "Entity/Weapon/Weapon.h"
#include <iostream>
#include "Manager/Texture/TextureManager.h"

Game* Game::mInstance = nullptr;

bool Game::CanPause()
{
	return mEntityManager.GetEntitiesWithComponents<DeathAnimationComponent>().empty();
}

void Game::InitKeyBindings()
{
	screen.AddKeyBinding(KeyBinding(sf::Keyboard::Escape, [this]()
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
	Factory::InitSystems(mSystemManager, mWindow, mCamera, mIsPaused);
}

void Game::InitPlayer()
{
	Factory::CreatePlayer(mEntityManager, { mWindow.getSize().x / 2.f, mWindow.getSize().y / 2.f });
}

void Game::InitMap()
{
	mMap.Init("assets/map/Grass.png");
}

void Game::LoadFont()
{
	font = TextureManager::GetFont("assets/font/Roboto-Bold.ttf");
}

void Game::RenderPauseScreen()
{
	screen.Clear();
	if (!mIsPaused)
	{
		return;
	}

	Button continueButton;
	continueButton
		.SetSize({ 200.f, 50.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText("Продолжить", font, 18)
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, 250.f })
		.SetOnClickListener([this]()
			{
				Resume();
				screen.Clear();
			});

	screen.AddView(std::make_shared<Button>(continueButton));

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Выйти", font, 18)
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, 310.f })
		.SetOnClickListener([this]()
			{
				auto players = mEntityManager.GetEntitiesWithComponents<PlayerHealthComponent>();
				if (players.empty())
				{
					return;
				}
				auto player = players.front();
				player->GetComponent<PlayerHealthComponent>()->Kill();
				Resume();
				screen.Clear();
			});
	screen.AddView(std::make_shared<Button>(exitButton));
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
	screen.Clear();
	screen.ClearBindings();
	mEntityManager.Clear();
	mSystemManager.Clear();
	mIsPaused = false;
	mConfig->maxBosses = bossCount == -1 ? mConfig->maxBosses : bossCount;
	mConfig->killedBosses = 0;
	mConfig->killedEnemies = 0;
}

void Game::Render(float deltaTime)
{
	mWindow.setView(mCamera);

	mWindow.draw(mMap);

	for (auto& system : mSystemManager.GetRenderSystems())
	{
		system->Render(mEntityManager, deltaTime);
	}

	mWindow.draw(screen);
}

void Game::ProcessEvents(const sf::Event& event)
{
	screen.HandleEvents(mWindow, mCamera, event);
}

bool Game::IsBossDefeated() const
{
	return mConfig->killedBosses >= mConfig->maxBosses && !mConfig->isInfinite;
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