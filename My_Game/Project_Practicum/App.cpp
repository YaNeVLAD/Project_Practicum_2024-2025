#include "App.h"

#include "ui/Button/Button.h"
#include "src/Manager/Texture/TextureManager.h"
#include "src/Factory/Factory.h"
#include "ui/Text/Text.h"

void App::Run()
{
	while (mWindow.isOpen())
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
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}

		if (mCurrentState == AppState::Playing)
		{
			mGame.ProcessEvents(event);
		}
		else
		{
			mScreen.HandleEvents(mWindow, mCamera, event);
		}
	}
}

void App::Update(float deltaTime)
{
	if (mCurrentState == AppState::Playing)
	{
		mGame.RunFrame(deltaTime);

		if (mGame.IsPlayerDefeated())
		{
			mCurrentState = AppState::Defeat;
			return;
		}

		if (mGame.HasPlayerLeveledUp())
		{
			mAvailableWeapons = mGame.GetAvailableWeapons();
			mCurrentState = AppState::WeaponUpgrade;
			return;
		}

		if (mGame.IsBossDefeated())
		{
			mCurrentState = AppState::Victory;
			return;
		}
	}
}

void App::Render(float deltaTime)
{
	mWindow.clear();
	mScreen.ClearBindings();

	switch (mCurrentState)
	{
	case AppState::MainMenu:
		RenderMainMenuScreen();
		break;
	case AppState::Victory:
		RenderVictoryScreen();
		break;
	case AppState::Defeat:
		RenderDefeatScreen();
		break;
	case AppState::WeaponUpgrade:
		mGame.Render(0.0f);
		RenderUpgradeScreen();
		break;
	case AppState::GameSetup:
		RenderGameSetupScreen();
		break;
	case AppState::Playing:
		mScreen.Clear();
		mGame.Render(deltaTime);
		break;
	}

	mWindow.draw(mScreen);
	mWindow.display();
}

void App::RenderUpgradeScreen()
{
	mGame.Pause();

	mScreen.Clear();

	if (mAvailableWeapons.empty())
	{
		mGame.Resume();
		mCurrentState = AppState::Playing;
		return;
	}

	sf::Vector2f buttonSize(200.0f, 50.0f);
	float spacing = 10.0f;

	for (int i = 0; i < mAvailableWeapons.size(); ++i)
	{
		Button button;
		button
			.SetSize(buttonSize)
			.SetFillColor(sf::Color::Yellow)
			.SetPosition(View::Alignment::Center, mCamera, { 0.0f, i * (buttonSize.y + spacing) })
			.SetText(mAvailableWeapons[i], mFont, 20, sf::Color::Black)
			.SetOnClickListener([this, weaponName = mAvailableWeapons[i]]()
				{
					mGame.UpgradeWeapon(weaponName);
					mCurrentState = AppState::Playing;
					mGame.Resume();
				});

		mScreen.AddView(std::make_shared<Button>(button));
	}
}

void App::RenderMainMenuScreen()
{
	mScreen.Clear();

	Button setupButton;
	setupButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera, { -160.f, 0.f })
		.SetFillColor(sf::Color::Green)
		.SetText("Start Game", mFont)
		.SetOnClickListener([this]()
			{
				mCurrentState = AppState::GameSetup;
			});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, mCamera, { 160.f, 0.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Exit Game", mFont)
		.SetOnClickListener([this]()
			{
				mWindow.close();
			});

	mScreen.AddView(std::make_shared<Button>(setupButton));
	mScreen.AddView(std::make_shared<Button>(exitButton));
}

using Key = sf::Keyboard::Key;
void App::RenderGameSetupScreen()
{
	mScreen.Clear();

	sf::Vector2f counterSize(75, 50);

	Button bossCount;
	bossCount
		.SetSize(counterSize)
		.SetFillColor(sf::Color::Yellow)
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, 0.f })
		.SetText(std::to_string(mGame.GetMaxBosses()), mFont, 20, sf::Color::Black);

	Button decreaseBossesButton;
	decreaseBossesButton
		.SetSize({ 50.f, 50.f })
		.SetPosition(View::Alignment::Center, mCamera, { -counterSize.x + 15, 0.f })
		.SetFillColor(sf::Color::White)
		.SetText("-", mFont, 20, sf::Color::Black)
		.SetOnClickListener([this]()
			{
				mGame.ChangeMaxBosses(-1);
			});

	Button increaseBossesButton;
	increaseBossesButton
		.SetSize({ 50.f, 50.f })
		.SetPosition(View::Alignment::Center, mCamera, { counterSize.x - 15, 0.f })
		.SetFillColor(sf::Color::White)
		.SetText("+", mFont, 20, sf::Color::Black)
		.SetOnClickListener([this]()
			{
				mGame.ChangeMaxBosses(1);
			});

	Button startButton;
	startButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, 200.f })
		.SetFillColor(sf::Color::Green)
		.SetText("Start", mFont)
		.SetOnClickListener([this]()
			{
				mGame.Restart();
				mCurrentState = AppState::Playing;
			});

	Button backButton;
	backButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, 260.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText("Back", mFont)
		.SetOnClickListener([this]()
			{
				mCurrentState = AppState::MainMenu;
			});

	KeyBinding increase({ Key::Equal, Key::D, Key::Right, Key::Space }, KeyBinding::OR, [this]()
		{
			mGame.ChangeMaxBosses(1);
		});

	KeyBinding decrease({ Key::Hyphen, Key::A, Key::Left, Key::Tab, Key::Backspace }, KeyBinding::OR, [this]()
		{
			mGame.ChangeMaxBosses(-1);
		});

	KeyBinding ret(Key::Escape, [this]()
		{
			mCurrentState = AppState::MainMenu;
		});

	mScreen.AddView(std::make_shared<Button>(bossCount));
	mScreen.AddView(std::make_shared<Button>(backButton));
	mScreen.AddView(std::make_shared<Button>(startButton));
	mScreen.AddView(std::make_shared<Button>(increaseBossesButton));
	mScreen.AddView(std::make_shared<Button>(decreaseBossesButton));

	mScreen.AddKeyBinding(increase);
	mScreen.AddKeyBinding(decrease);
	mScreen.AddKeyBinding(ret);
}

void App::RenderVictoryScreen()
{
	mScreen.Clear();

	Button mainMenuButton;
	mainMenuButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera)
		.SetFillColor(sf::Color::Green)
		.SetText("Main Menu", mFont)
		.SetOnClickListener([this]()
			{
				mCurrentState = AppState::MainMenu;
			});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, 120.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Exit Game", mFont)
		.SetOnClickListener([this]()
			{
				mWindow.close();
			});

	mScreen.AddView(std::make_shared<Button>(mainMenuButton));
	mScreen.AddView(std::make_shared<Button>(exitButton));
}

void App::RenderDefeatScreen()
{
	mScreen.Clear();

	Button mainMenuButton;
	mainMenuButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera)
		.SetFillColor(sf::Color::Green)
		.SetText("Main Menu", mFont)
		.SetOnClickListener([this]()
			{
				mCurrentState = AppState::MainMenu;
			});

	Button restartButton;
	restartButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera, { 0.0f, 60.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText("Restart", mFont)
		.SetOnClickListener([this]()
			{
				mGame.Restart();
				mCurrentState = AppState::Playing;
			});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, 120.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Exit Game", mFont)
		.SetOnClickListener([this]()
			{
				mWindow.close();
			});

	mScreen.AddView(std::make_shared<Button>(restartButton));
	mScreen.AddView(std::make_shared<Button>(mainMenuButton));
	mScreen.AddView(std::make_shared<Button>(exitButton));
}

void App::LoadFont()
{
	mFont = TextureManager::GetFont("assets/font/Roboto-Bold.ttf");
}

void App::LoadTextures()
{
	Factory::LoadTextures();
}