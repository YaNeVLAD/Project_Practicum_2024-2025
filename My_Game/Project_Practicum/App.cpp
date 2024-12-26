#include "App.h"

#include "ui/Button/Button.h"
#include "src/Factory/Factory.h"
#include "ui/Text/Text.h"
#include "src/Manager/Texture/TextureManager.h"


using Key = sf::Keyboard::Key;
void App::Run()
{
	while (window.isOpen())
	{
		float deltaTime = mClock.restart().asSeconds();
		ProcessEvents();
		Update(deltaTime);
		Render(deltaTime);
	}
}

void App::ProcessEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}

		if (state == State::Playing)
		{
			game->ProcessEvents(event);
		}
		else
		{
			screen.HandleEvents(window, camera, event);
		}
	}
}

void App::Update(float deltaTime)
{
	if (state == State::Playing)
	{
		game->RunFrame(deltaTime);

		if (game->IsPlayerDefeated())
		{
			mConfig->SaveConfig();
			state = State::Defeat;
			return;
		}

		if (game->HasPlayerLeveledUp())
		{
			mConfig->SaveConfig();
			mAvailableWeapons = game->GetAvailableWeapons();
			state = State::WeaponUpgrade;
			return;
		}

		if (game->IsBossDefeated())
		{
			mConfig->SaveConfig();
			state = State::Victory;
			return;
		}
	}
	if (state == State::WeaponUpgrade)
		for (const auto& sprite : mWeaponSprites)
			sprite.second->Update(deltaTime * 5);
	if (state == State::GameSetup)
		for (const auto& player : mPlayers)
			player.second->Update(deltaTime * 5);
}

void App::Render(float deltaTime)
{
	window.clear();
	screen.ClearBindings();

	switch (state)
	{
	case State::MainMenu:
		RenderMainMenuScreen();
		break;
	case State::Victory:
		RenderVictoryScreen();
		break;
	case State::Defeat:
		RenderDefeatScreen();
		break;
	case State::WeaponUpgrade:
		game->Render(0.0f);
		RenderUpgradeScreen();
		break;
	case State::GameSetup:
		RenderGameSetupScreen();
		break;
	case State::Playing:
		screen.Clear();
		game->Render(deltaTime);
		break;
	case State::UpgradeShop:
		RenderUpgradeShopScreen();
		break;
	}

	window.draw(screen);
	window.display();
}

void App::RenderUpgradeScreen()
{
	game->Pause();

	screen.Clear();

	if (mAvailableWeapons.empty())
	{
		game->Resume();
		state = State::Playing;
		return;
	}

	sf::Vector2f buttonSize(200.0f, 50.0f);
	float spacing = 10.0f;
	float buttonWidth = buttonSize.x + spacing;

	float screenCenterX = window.getSize().x / static_cast<float>(16);
	float startPosX = screenCenterX - ((mAvailableWeapons.size() * buttonWidth) / 2);
	for (int i = 0; i < mAvailableWeapons.size(); ++i)
	{
		auto& weapon = mAvailableWeapons[i];

		if (mWeaponSprites.find(weapon->GetName()) == mWeaponSprites.end())
		{
			auto sprite = std::make_shared<AnimatedSprite>();
			sprite->SetSize({ 200.f, 200.f })
				.SetBackgroundColor(sf::Color::White)
				.SetTextures(*weapon->GetAnimation());

			mWeaponSprites[weapon->GetName()] = sprite;
		}

		auto& sprite = *mWeaponSprites[weapon->GetName()];
		sprite.SetPosition(View::Alignment::Center, camera, { startPosX + i * buttonWidth, 35.f });

		Button button;
		button
			.SetSize(buttonSize)
			.SetFillColor(sf::Color::Yellow)
			.SetPosition(View::Alignment::Center, camera, { startPosX + i * buttonWidth, 160.f })
			.SetText(weapon->GetName() + (weapon->GetLevel() == 0 ? "" : " " + std::to_string(weapon->GetLevel())), mFont, 20, sf::Color::Black)
			.SetOnClickListener([this, weapon]()
				{
					game->UpgradeWeapon(weapon->GetName());
					state = State::Playing;
					game->Resume();
				});

		screen.AddView(std::make_shared<AnimatedSprite>(sprite));
		screen.AddView(std::make_shared<Button>(button));
	}
}

void App::RenderMainMenuScreen()
{
	screen.Clear();

	Button setupButton;
	setupButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera, { -160.f, 0.f })
		.SetFillColor(sf::Color::Green)
		.SetText("Начать игру", mFont)
		.SetOnClickListener([this]()
			{
				state = State::GameSetup;
			});

	Button upgradeButton;
	upgradeButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, 60.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText(std::to_string(mConfig->upgradePoints), mFont)
		.SetOnClickListener([this]()
			{
				state = State::UpgradeShop;
			});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { 160.f, 0.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Выйти из игры", mFont)
		.SetOnClickListener([this]()
			{
				window.close();
			});

	screen.AddView(std::make_shared<Button>(setupButton));
	screen.AddView(std::make_shared<Button>(upgradeButton));
	screen.AddView(std::make_shared<Button>(exitButton));
}

void App::RenderGameSetupScreen()
{
	screen.Clear();

	sf::Vector2f counterSize(75, 50);

	auto firstPlayerTextures = TextureManager::GetTextures("assets/character/1/Idle.png", 128, 128);
	auto secondPlayerTextures = TextureManager::GetTextures("assets/character/2/Idle.png", 128, 128);

	auto firstPlayerWeapon = TextureManager::GetTextures("assets/weapon/Charge.png", 64, 64);
	auto secondPlayerWeapon = TextureManager::GetTextures("assets/weapon/Fireball.png", 64, 64);

	std::vector<GameConfig::PlayerType> playerTypes = { GameConfig::PlayerType::First, GameConfig::PlayerType::Second };

	float buttonWidth = 200.f;
	float buttonHeight = 50.f;
	float spacing = 20.f;

	for (int i = 0; i < playerTypes.size(); ++i)
	{
		const auto& playerType = playerTypes[i];

		if (mPlayers.find(playerType) == mPlayers.end())
		{
			auto sprite = std::make_shared<AnimatedSprite>();
			sprite->SetSize({ 128.f, 128.f });
			sprite->SetTextures(playerType == GameConfig::PlayerType::First ? *firstPlayerTextures : *secondPlayerTextures);
			mPlayers[playerType] = sprite;
		}

		auto& sprite = mPlayers[playerType];

		float offsetX = (i == 0) ? -120.f : 120.f;
		sprite->SetPosition(View::Alignment::Center, camera, { offsetX, 0 });

		Button selectButton;
		selectButton
			.SetSize({ buttonWidth, buttonHeight })
			.SetFillColor(playerType == mConfig->playerType ? sf::Color::Green : sf::Color::Red)
			.SetPosition(View::Alignment::Center, camera, { offsetX, 220.f - 100.f })
			.SetText("Select", mFont, 20, playerType == mConfig->playerType ? sf::Color::White : sf::Color::Black)
			.SetOnClickListener([this, playerType]()
				{
					mConfig->playerType = playerType;
				});

		screen.AddView(sprite);
		screen.AddView(std::make_shared<Button>(selectButton));
	}

	Button bossCount;
	bossCount
		.SetSize(counterSize)
		.SetFillColor(sf::Color::Yellow)
		.SetPosition(View::Alignment::Center, camera, { 0.f, 300.f - 100.f })
		.SetText(std::to_string(GameConfig::GetInstance()->maxBosses), mFont, 20, sf::Color::Black);

	Button decreaseBossesButton;
	decreaseBossesButton
		.SetSize({ 50.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { -counterSize.x + 15, 300.f - 100.f })
		.SetFillColor(sf::Color::White)
		.SetText("-", mFont, 20, sf::Color::Black)
		.SetOnClickListener([this]()
			{
				if (mConfig->maxBosses > 1)
					mConfig->maxBosses--;
			});

	Button increaseBossesButton;
	increaseBossesButton
		.SetSize({ 50.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { counterSize.x - 15, 300.f - 100.f })
		.SetFillColor(sf::Color::White)
		.SetText("+", mFont, 20, sf::Color::Black)
		.SetOnClickListener([this]()
			{
				if (mConfig->maxBosses < 999)
					mConfig->maxBosses++;
			});

	Button infiniteModeButton;
	infiniteModeButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, 380.f - 100.f })
		.SetFillColor(mConfig->isInfinite ? sf::Color::Blue : sf::Color::White)
		.SetText("Бесконечный режим", mFont, 20, sf::Color::Black)
		.SetOnClickListener([this]()
			{
				mConfig->isInfinite = !mConfig->isInfinite;
			});

	screen.AddView(std::make_shared<Button>(bossCount));
	screen.AddView(std::make_shared<Button>(decreaseBossesButton));
	screen.AddView(std::make_shared<Button>(increaseBossesButton));
	screen.AddView(std::make_shared<Button>(infiniteModeButton));

	Button startButton;
	startButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, 440.f - 100.f })
		.SetFillColor(sf::Color::Green)
		.SetText("НАЧАТЬ (Enter)", mFont)
		.SetOnClickListener([this]()
			{
				game->Restart(mConfig->maxBosses);
				state = State::Playing;
			});

	Button backButton;
	backButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, 500.f - 100.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText("Назад (Esc)", mFont)
		.SetOnClickListener([this]()
			{
				state = State::MainMenu;
			});

	screen.AddView(std::make_shared<Button>(startButton));
	screen.AddView(std::make_shared<Button>(backButton));

	KeyBinding increase({ Key::Equal, Key::D, Key::Right }, KeyBinding::OR, [this]()
		{
			if (mConfig->maxBosses < 999)
				mConfig->maxBosses++;
		});

	KeyBinding decrease({ Key::Hyphen, Key::A, Key::Left }, KeyBinding::OR, [this]()
		{
			if (mConfig->maxBosses > 1)
				mConfig->maxBosses--;
		});

	KeyBinding ret(Key::Escape, [this]()
		{
			state = State::MainMenu;
		});

	KeyBinding start(Key::Enter, [this]()
		{
			game->Restart();
			state = State::Playing;
		});

	screen.AddKeyBinding(ret);
	screen.AddKeyBinding(start);
	screen.AddKeyBinding(increase);
	screen.AddKeyBinding(decrease);
}

void App::RenderUpgradeShopScreen()
{
	screen.Clear();

	auto config = GameConfig::GetInstance();
	sf::Vector2f buttonSize(200.0f, 50.0f);
	float spacing = 20.0f;

	float screenWidth = window.getSize().x / static_cast<float>(16);
	float rowCenterX = screenWidth / 2;

	Button healthBuffButton;
	healthBuffButton
		.SetSize(buttonSize)
		.SetPosition(View::Alignment::Center, camera, { rowCenterX - buttonSize.x - spacing / 2, 100.0f })
		.SetFillColor(sf::Color::Green)
		.SetText("Increase Health", mFont, 20, sf::Color::White)
		.SetOnClickListener([config]() {
		if (config->upgradePoints > 0) {
			config->playerHealthBuff++;
			config->upgradePoints--;
		}
			});

	Button damageBuffButton;
	damageBuffButton
		.SetSize(buttonSize)
		.SetPosition(View::Alignment::Center, camera, { rowCenterX + spacing / 2, 100.0f })
		.SetFillColor(sf::Color::Red)
		.SetText("Increase Damage", mFont, 20, sf::Color::White)
		.SetOnClickListener([config]() {
		if (config->upgradePoints > 0) {
			config->playerDamageBuff++;
			config->upgradePoints--;
		}
			});

	screen.AddView(std::make_shared<Button>(healthBuffButton));
	screen.AddView(std::make_shared<Button>(damageBuffButton));

	float secondRowY = 200.0f;
	float startPosX = rowCenterX - 2 * (buttonSize.x + spacing) / 2;
	std::vector<std::string> weaponKeys = {
		config->AxeDamage,
		config->ChargeDamage,
		config->LightningDamage,
		config->FireballDamage,
		config->BookDamage
	};

	for (size_t i = 0; i < weaponKeys.size(); ++i) {
		std::string weaponName = weaponKeys[i];
		Button weaponButton;
		weaponButton
			.SetSize(buttonSize)
			.SetPosition(View::Alignment::Center, camera, { startPosX + i * (buttonSize.x + spacing), secondRowY })
			.SetFillColor(sf::Color::Yellow)
			.SetText(weaponName, mFont, 20, sf::Color::Black)
			.SetOnClickListener([config, weaponName]() {
			if (config->upgradePoints > 0) {
				config->weaponStats[weaponName]++;
				config->upgradePoints--;
			}
				});

		screen.AddView(std::make_shared<Button>(weaponButton));
	}

	Button mainMenuButton;
	mainMenuButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, -300.f })
		.SetFillColor(sf::Color::Green)
		.SetText("В главное меню", mFont)
		.SetOnClickListener([this]() {
		mConfig->SaveConfig();
		state = State::MainMenu;
			});

	Button upgradeButton;
	upgradeButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, -360.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText(std::to_string(mConfig->upgradePoints), mFont);

	screen.AddView(std::make_shared<Button>(mainMenuButton));
	screen.AddView(std::make_shared<Button>(upgradeButton));
}

void App::RenderVictoryScreen()
{
	screen.Clear();

	Text victoryText;
	victoryText
		.SetText("Вы победили!", mFont, 60, sf::Color::Green)
		.SetPosition(View::Alignment::Center, camera, { 0.f, -200.f })
		.SetTextAlignment(Text::TextAlignment::Center);

	Text killedEnemies;
	killedEnemies
		.SetText("Убитых врагов: " + std::to_string(GameConfig::GetInstance()->killedEnemies + GameConfig::GetInstance()->killedBosses), mFont, 24)
		.SetPosition(View::Alignment::Center, camera, { 0.f, -100.f });

	Button mainMenuButton;
	mainMenuButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera)
		.SetFillColor(sf::Color::Green)
		.SetText("В главное меню (Esc)", mFont)
		.SetOnClickListener([this]()
			{
				state = State::MainMenu;
			});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, 120.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Выйти из игры", mFont)
		.SetOnClickListener([this]()
			{
				window.close();
			});

	KeyBinding exit(sf::Keyboard::Escape, [this]()
		{
			state = State::MainMenu;
		});

	screen.AddView(std::make_shared<Text>(killedEnemies));
	screen.AddView(std::make_shared<Text>(victoryText));
	screen.AddView(std::make_shared<Button>(mainMenuButton));
	screen.AddView(std::make_shared<Button>(exitButton));

	screen.AddKeyBinding(exit);
}

void App::RenderDefeatScreen()
{
	screen.Clear();

	Text defeatText;
	defeatText
		.SetText("Вы проиграли!", mFont, 60, sf::Color::Red)
		.SetPosition(View::Alignment::Center, camera, { 0.f, -200.f })
		.SetTextAlignment(Text::TextAlignment::Center);

	Text killedEnemies;
	killedEnemies
		.SetText("Убитых врагов: " + std::to_string(GameConfig::GetInstance()->killedEnemies + GameConfig::GetInstance()->killedBosses), mFont, 24)
		.SetPosition(View::Alignment::Center, camera, { 0.f, -100.f });

	Button mainMenuButton;
	mainMenuButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera)
		.SetFillColor(sf::Color::Green)
		.SetText("В главное меню (Esc)", mFont)
		.SetOnClickListener([this]()
			{
				state = State::MainMenu;
			});

	Button restartButton;
	restartButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera, { 0.0f, 60.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText("Заново (R)", mFont)
		.SetOnClickListener([this]()
			{
				game->Restart();
				state = State::Playing;
			});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, 120.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Выйти из игры", mFont)
		.SetOnClickListener([this]()
			{
				window.close();
			});


	KeyBinding restart(sf::Keyboard::R, [this]()
		{
			game->Restart();
			state = State::Playing;
		});

	KeyBinding exit(sf::Keyboard::Escape, [this]()
		{
			state = State::MainMenu;
		});

	screen.AddView(std::make_shared<Text>(killedEnemies));
	screen.AddView(std::make_shared<Text>(defeatText));
	screen.AddView(std::make_shared<Button>(restartButton));
	screen.AddView(std::make_shared<Button>(mainMenuButton));
	screen.AddView(std::make_shared<Button>(exitButton));

	screen.AddKeyBinding(restart);
	screen.AddKeyBinding(exit);
}

void App::LoadFont()
{
	mFont = TextureManager::GetFont("assets/font/Roboto-Bold.ttf");
}

void App::LoadTextures()
{
	Factory::LoadTextures();
}