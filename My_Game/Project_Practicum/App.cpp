#include "App.h"

#include <iostream>
#include "ui/Button/Button.h"
#include "src/Manager/Texture/TextureManager.h"
#include "src/Factory/Factory.h"

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

	switch (mCurrentState)
	{
	case AppState::MainMenu:
		InitMainMenuScreen();
		mWindow.draw(mScreen);
		break;
	case AppState::Victory:
		InitVictoryScreen();
		mWindow.draw(mScreen);
		break;
	case AppState::Defeat:
		InitDefeatScreen();
		mWindow.draw(mScreen);
		break;
	case AppState::WeaponUpgrade:
		mGame.Render(0.0f);
		InitUpgradeScreen();
		mWindow.draw(mScreen);
		break;
	case AppState::Playing:
		mGame.Render(deltaTime);
		break;
	}

	mWindow.display();
}

void App::InitUpgradeScreen()
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
			.SetText(mAvailableWeapons[i], mFont, 20, sf::Color::Black);

		button.SetOnClickListener([this, weaponName = mAvailableWeapons[i]]()
			{
				mGame.UpgradeWeapon(weaponName);
				mCurrentState = AppState::Playing;
				mGame.Resume();
			});

		mScreen.AddView(std::make_shared<Button>(button));
	}
}

void App::InitMainMenuScreen()
{
	mScreen.Clear();

	Button startButton;
	startButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera, {-160.f, 0.f})
		.SetFillColor(sf::Color::Green)
		.SetText("Start Game", mFont);

	startButton.SetOnClickListener([this]()
		{
			mGame.Restart();
			mCurrentState = AppState::Playing;
		});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, mCamera, { 160.f, 0.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Exit Game", mFont);

	exitButton.SetOnClickListener([this]()
		{
			mWindow.close();
		});

	mScreen.AddView(std::make_shared<Button>(startButton));
	mScreen.AddView(std::make_shared<Button>(exitButton));
}

void App::InitVictoryScreen()
{
	mScreen.Clear();

	Button mainMenuButton;
	mainMenuButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera)
		.SetFillColor(sf::Color::Green)
		.SetText("Main Menu", mFont);

	mainMenuButton.SetOnClickListener([this]()
		{
			mCurrentState = AppState::MainMenu;
		});

	Button restartButton;
	restartButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera, { 0.0f, 60.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText("Restart", mFont);

	restartButton.SetOnClickListener([this]()
		{
			mGame.Restart();
			mCurrentState = AppState::Playing;
		});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, 120.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Exit Game", mFont);

	exitButton.SetOnClickListener([this]()
		{
			mWindow.close();
		});

	mScreen.AddView(std::make_shared<Button>(restartButton));
	mScreen.AddView(std::make_shared<Button>(mainMenuButton));
	mScreen.AddView(std::make_shared<Button>(exitButton));
}

void App::InitDefeatScreen()
{
	mScreen.Clear();

	Button mainMenuButton;
	mainMenuButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera)
		.SetFillColor(sf::Color::Green)
		.SetText("Main Menu", mFont);

	mainMenuButton.SetOnClickListener([this]()
		{
			mCurrentState = AppState::MainMenu;
		});

	Button restartButton;
	restartButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(View::Alignment::Center, mCamera, { 0.0f, 60.f })
		.SetFillColor(sf::Color::Yellow)
		.SetText("Restart", mFont);

	restartButton.SetOnClickListener([this]()
		{
			mGame.Restart();
			mCurrentState = AppState::Playing;
		});

	Button exitButton;
	exitButton
		.SetSize({ 200.f, 50.f })
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, 120.f })
		.SetFillColor(sf::Color::Red)
		.SetText("Exit Game", mFont);

	exitButton.SetOnClickListener([this]()
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