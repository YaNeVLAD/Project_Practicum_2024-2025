#include "App.h"

#include <iostream>
#include "ui/Button/Button.h"

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
		switch (event.type)
		{
		case sf::Event::Closed: mWindow.close(); break;
		}

		if (mCurrentState == AppState::Playing)
		{
			mGame.ProcessEvents();
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
		}

		if (mGame.HasPlayerLeveledUp())
		{
			InitUpgradeScreen();
			mCurrentState = AppState::WeaponUpgrade;
		}

		if (mGame.IsBossDefeated())
		{
			mCurrentState = AppState::Victory;
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
	mGame.PauseGame();

	mScreen.Clear();

	if (!mFont.loadFromFile("assets/font/Roboto-Bold.ttf"))
	{
		throw std::runtime_error("Failed to load font");
	}

	auto availableWeapons = mGame.GetAvailableWeapons();
	if (availableWeapons.empty())
	{
		mCurrentState = AppState::Playing;
		mGame.ResumeGame();
		return;
	}

	sf::Vector2f buttonSize(200.0f, 50.0f);
	float spacing = 10.0f;

	for (size_t i = 0; i < availableWeapons.size(); ++i)
	{
		Button button;
		button
			.SetSize(buttonSize)
			.SetFillColor(sf::Color::Yellow)
			.SetPosition(Button::Alignment::CenterX, mCamera, { 0.0f, i * (buttonSize.y + spacing) })
			.SetText(availableWeapons[i], mFont, 20, sf::Color::Black);

		button.SetOnClickListener([this, weaponName = availableWeapons[i]]()
			{
				mGame.UpgradeWeapon(weaponName);
				mCurrentState = AppState::Playing;
				mGame.ResumeGame();
			});

		mScreen.AddView(std::make_shared<Button>(button));
	}
}

void App::InitMainMenuScreen()
{
	mScreen.Clear();

	if (!mFont.loadFromFile("assets/font/Roboto-Bold.ttf"))
	{
		throw std::runtime_error("Failed to load font");
	}

	Button startButton;
	startButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(Button::Alignment::Center, mCamera, { 0.0f, 0.f })
		.SetFillColor(sf::Color::Green)
		.SetText("Start Game", mFont);

	startButton.SetOnClickListener([this]()
		{
			mGame.Restart();
			mCurrentState = AppState::Playing;
		});

	mScreen.AddView(std::make_shared<Button>(startButton));
}

void App::InitVictoryScreen()
{
	mScreen.Clear();

	if (!mFont.loadFromFile("assets/font/Roboto-Bold.ttf"))
	{
		throw std::runtime_error("Failed to load font");
	}

	Button exitButton;
	exitButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(Button::Alignment::Center, mCamera)
		.SetFillColor(sf::Color::Red)
		.SetText("Main Menu", mFont);

	exitButton.SetOnClickListener([this]()
		{
			mCurrentState = AppState::MainMenu;
		});

	Button restartButton;
	restartButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(Button::Alignment::Center, mCamera, { 0.0f, 60.f })
		.SetFillColor(sf::Color::Green)
		.SetText("Restart", mFont);

	restartButton.SetOnClickListener([this]()
		{
			mGame.Restart();
			mCurrentState = AppState::Playing;
		});

	mScreen.AddView(std::make_shared<Button>(restartButton));
	mScreen.AddView(std::make_shared<Button>(exitButton));
}

void App::InitDefeatScreen()
{
	mScreen.Clear();

	if (!mFont.loadFromFile("assets/font/Roboto-Bold.ttf"))
	{
		throw std::runtime_error("Failed to load font");
	}

	Button exitButton;
	exitButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(Button::Alignment::Center, mCamera)
		.SetFillColor(sf::Color::Red)
		.SetText("Main Menu", mFont);

	exitButton.SetOnClickListener([this]()
		{
			mCurrentState = AppState::MainMenu;
		});

	Button restartButton;
	restartButton
		.SetSize({ 200.0f, 50.0f })
		.SetPosition(Button::Alignment::Center, mCamera, { 0.0f, 60.f })
		.SetFillColor(sf::Color::Green)
		.SetText("Restart", mFont);

	restartButton.SetOnClickListener([this]()
		{
			mGame.Restart();
			mCurrentState = AppState::Playing;
		});

	mScreen.AddView(std::make_shared<Button>(restartButton));
	mScreen.AddView(std::make_shared<Button>(exitButton));
}