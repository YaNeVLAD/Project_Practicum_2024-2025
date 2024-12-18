#include "App.h"

#include "ui/Button/Button.h"
#include "src/Manager/Texture/TextureManager.h"
#include "src/Factory/Factory.h"

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
			game.ProcessEvents(event);
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
		game.RunFrame(deltaTime);

		if (game.IsPlayerDefeated())
		{
			state = State::Defeat;
			return;
		}

		if (game.HasPlayerLeveledUp())
		{
			mAvailableWeapons = game.GetAvailableWeapons();
			state = State::WeaponUpgrade;
			return;
		}

		if (game.IsBossDefeated())
		{
			state = State::Victory;
			return;
		}
	}
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
		game.Render(0.0f);
		RenderUpgradeScreen();
		break;
	case State::GameSetup:
		RenderGameSetupScreen();
		break;
	case State::Playing:
		screen.Clear();
		game.Render(deltaTime);
		break;
	}

	window.draw(screen);
	window.display();
}

void App::RenderUpgradeScreen()
{
	game.Pause();

	screen.Clear();

	if (mAvailableWeapons.empty())
	{
		game.Resume();
		state = State::Playing;
		return;
	}

	sf::Vector2f buttonSize(200.0f, 50.0f);
	float spacing = 10.0f;

	for (int i = 0; i < mAvailableWeapons.size(); ++i)
	{
		auto& weapon = mAvailableWeapons[i];

		Button button;
		button
			.SetSize(buttonSize)
			.SetFillColor(sf::Color::Yellow)
			.SetPosition(View::Alignment::Center, camera, { 0.0f, i * (buttonSize.y + spacing) })
			.SetText(weapon->GetName() + (weapon->GetLevel() == 0 ? "" : " " + std::to_string(weapon->GetLevel())), mFont, 20, sf::Color::Black)
			.SetOnClickListener([this, weapon]()
				{
					game.UpgradeWeapon(weapon->GetName());
					state = State::Playing;
					game.Resume();
				});

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
		.SetText("Start Game", mFont)
		.SetOnClickListener([this]()
			{
				state = State::GameSetup;
			});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { 160.f, 0.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Exit Game", mFont)
		.SetOnClickListener([this]()
			{
				window.close();
			});

	screen.AddView(std::make_shared<Button>(setupButton));
	screen.AddView(std::make_shared<Button>(exitButton));
}

using Key = sf::Keyboard::Key;
void App::RenderGameSetupScreen()
{
	screen.Clear();

	sf::Vector2f counterSize(75, 50);

	Button bossCount;
	bossCount
		.SetSize(counterSize)
		.SetFillColor(sf::Color::Yellow)
		.SetPosition(View::Alignment::Center, camera, { 0.f, 0.f })
		.SetText(std::to_string(game.GetMaxBosses()), mFont, 20, sf::Color::Black);

	Button decreaseBossesButton;
	decreaseBossesButton
		.SetSize({ 50.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { -counterSize.x + 15, 0.f })
		.SetFillColor(sf::Color::White)
		.SetText("-", mFont, 20, sf::Color::Black)
		.SetOnClickListener([this]()
			{
				game.ChangeMaxBosses(-1);
			});

	Button increaseBossesButton;
	increaseBossesButton
		.SetSize({ 50.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { counterSize.x - 15, 0.f })
		.SetFillColor(sf::Color::White)
		.SetText("+", mFont, 20, sf::Color::Black)
		.SetOnClickListener([this]()
			{
				game.ChangeMaxBosses(1);
			});

	Button startButton;
	startButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, 200.f })
		.SetFillColor(sf::Color::Green)
		.SetText("Start", mFont)
		.SetOnClickListener([this]()
			{
				game.Restart();
				state = State::Playing;
			});

	Button backButton;
	backButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, 260.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText("Back", mFont)
		.SetOnClickListener([this]()
			{
				state = State::MainMenu;
			});

	Button infiniteModeButton;
	infiniteModeButton
		.SetSize({ 300.f, 100.f })
		.SetFillColor(game.isInfinite ? sf::Color::Green : sf::Color::Red)
		.SetText("Infinite Mode", mFont)
		.SetPosition(View::Alignment::Center, camera, { 0.f, 360.f })
		.SetOnClickListener([this]()
			{
				game.isInfinite = !game.isInfinite;
			});

	KeyBinding increase({ Key::Equal, Key::D, Key::Right, Key::Space }, KeyBinding::OR, [this]()
		{
			game.ChangeMaxBosses(1);
		});

	KeyBinding decrease({ Key::Hyphen, Key::A, Key::Left, Key::Tab, Key::Backspace }, KeyBinding::OR, [this]()
		{
			game.ChangeMaxBosses(-1);
		});

	KeyBinding ret(Key::Escape, [this]()
		{
			state = State::MainMenu;
		});

	KeyBinding start(Key::Enter, [this]()
		{
			game.Restart();
			state = State::Playing;
		});

	screen.AddView(std::make_shared<Button>(bossCount));
	screen.AddView(std::make_shared<Button>(backButton));
	screen.AddView(std::make_shared<Button>(startButton));
	screen.AddView(std::make_shared<Button>(infiniteModeButton));
	screen.AddView(std::make_shared<Button>(increaseBossesButton));
	screen.AddView(std::make_shared<Button>(decreaseBossesButton));

	screen.AddKeyBinding(ret);
	screen.AddKeyBinding(start);
	screen.AddKeyBinding(increase);
	screen.AddKeyBinding(decrease);
}

void App::RenderVictoryScreen()
{
	screen.Clear();

	Button mainMenuButton;
	mainMenuButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera)
		.SetFillColor(sf::Color::Green)
		.SetText("Main Menu", mFont)
		.SetOnClickListener([this]()
			{
				state = State::MainMenu;
			});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, 120.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Exit Game", mFont)
		.SetOnClickListener([this]()
			{
				window.close();
			});

	screen.AddView(std::make_shared<Button>(mainMenuButton));
	screen.AddView(std::make_shared<Button>(exitButton));
}

void App::RenderDefeatScreen()
{
	screen.Clear();

	Button mainMenuButton;
	mainMenuButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera)
		.SetFillColor(sf::Color::Green)
		.SetText("Main Menu", mFont)
		.SetOnClickListener([this]()
			{
				state = State::MainMenu;
			});

	Button restartButton;
	restartButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, camera, { 0.0f, 60.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText("Restart", mFont)
		.SetOnClickListener([this]()
			{
				game.Restart();
				state = State::Playing;
			});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, camera, { 0.f, 120.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Exit Game", mFont)
		.SetOnClickListener([this]()
			{
				window.close();
			});

	screen.AddView(std::make_shared<Button>(restartButton));
	screen.AddView(std::make_shared<Button>(mainMenuButton));
	screen.AddView(std::make_shared<Button>(exitButton));
}

void App::LoadFont()
{
	mFont = TextureManager::GetFont("assets/font/Roboto-Bold.ttf");
}

void App::LoadTextures()
{
	Factory::LoadTextures();
}