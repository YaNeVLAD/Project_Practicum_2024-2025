#include "App.h"
#include <iostream>

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

		if (mCurrentState == App::MainMenu)
		{
			HandleMainMenuEvents(event);
		}
		else if (mCurrentState == App::Playing)
		{
			mGame.ProcessEvents();
		}
		else if (mCurrentState == App::Defeat)
		{
			HandleDefeatScreenEvents(event);
		}
		else if (mCurrentState == App::WeaponUpgrade)
		{
			HandleWeaponUpgradeEvents(event);
		}
		else if (mCurrentState == App::Victory)
		{
			HandleVictoryScreenEvents(event);
		}
	}
}

void App::Update(float deltaTime)
{
	if (mCurrentState == App::Playing)
	{
		mGame.RunFrame(deltaTime);
		if (mGame.IsPlayerDefeated())
		{
			mCurrentState = App::Defeat;
		}

		if (mGame.HasPlayerLeveledUp())
		{
			mCurrentState = App::WeaponUpgrade;
			InitUpgradeScreen();
		}

		if (mGame.IsBossDefeated())
		{
			mCurrentState = App::Victory;
		}
	}
}

void App::Render(float deltaTime)
{
	mWindow.clear();

	switch (mCurrentState)
	{
	case App::MainMenu:
		RenderMainMenu();
		break;
	case App::Playing:
		mGame.Render(deltaTime);
		break;
	case Victory:
		RenderVictoryScreen();
		break;
	case App::Defeat:
		RenderDefeatScreen();
		break;
	case WeaponUpgrade:
		RenderWeaponUpgradeScreen();
		break;
	}

	mWindow.display();
}

void App::HandleMainMenuEvents(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed) 
	{
		mCurrentState = App::Playing;
		mGame.init();
	}
}

void App::HandleVictoryScreenEvents(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		mWindow.close();
	}
}

void App::HandleWeaponUpgradeEvents(const sf::Event& event)
{
	if (event.type != sf::Event::MouseButtonPressed)
	{
		return;
	}

	sf::Vector2f worldPos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mCamera);

	for (size_t i = 0; i < mWeaponUpgradeButtons.size(); ++i)
	{
		if (mWeaponUpgradeButtons[i].getGlobalBounds().contains(worldPos))
		{
			std::string name = mButtonTexts[i].getString();
			mGame.UpgradeWeapon(name);
			mCurrentState = Playing;
			mGame.ResumeGame();
		}
	}
}

void App::HandleDefeatScreenEvents(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		mWindow.close();
	}
}

void App::InitUpgradeScreen()
{
	mGame.PauseGame();
	mWeaponUpgradeButtons.clear();
	mButtonTexts.clear();

	if (!mFont.loadFromFile("assets/font/Roboto-Bold.ttf"))
	{
		throw std::runtime_error("Failed to load font");
	}

	sf::Vector2f buttonSize(200.0f, 50.0f);

	float spacing = 10.0f;

	auto availableWeapons = mGame.GetAvailableWeapons();

	if (availableWeapons.empty())
	{
		mCurrentState = App::Playing;
;		mGame.ResumeGame();
		return;
	}

	float totalHeight = availableWeapons.size() * buttonSize.y + (availableWeapons.size() - 1) * spacing;

	sf::Vector2f cameraCenter = mCamera.getCenter();
	sf::Vector2f cameraSize = mCamera.getSize();

	float startY = cameraCenter.y - totalHeight / 2.0f;

	for (size_t i = 0; i < availableWeapons.size(); ++i)
	{
		// метод длинный, вынести
		sf::RectangleShape button(buttonSize);
		button.setPosition(
			cameraCenter.x - buttonSize.x / 2.0f,
			startY + i * (buttonSize.y + spacing)
		);
		button.setFillColor(sf::Color::Yellow);
		mWeaponUpgradeButtons.push_back(button);

		sf::Text buttonText;
		buttonText.setFont(mFont);
		buttonText.setString(availableWeapons[i]);
		buttonText.setCharacterSize(20);
		buttonText.setFillColor(sf::Color::Black);

		sf::FloatRect textBounds = buttonText.getLocalBounds();
		buttonText.setPosition(
			button.getPosition().x + (buttonSize.x - textBounds.width) / 2.0f,
			button.getPosition().y + (buttonSize.y - textBounds.height) / 2.0f - textBounds.top
		);
		mButtonTexts.push_back(buttonText);
	}
}

void App::RenderMainMenu()
{
	mWindow.draw(mMainMenuText);
}

void App::RenderDefeatScreen()
{
	mWindow.draw(mDefeatText);
	mWindow.draw(mExitButton);
}

void App::RenderVictoryScreen()
{
	mWindow.draw(mDefeatText);
	mWindow.draw(mExitButton);
}

void App::RenderWeaponUpgradeScreen()
{
	mGame.Render(0.0f);
	for (auto& button : mWeaponUpgradeButtons)
	{
		mWindow.draw(button);
	}

	for (auto& text : mButtonTexts)
	{
		mWindow.draw(text);
	}
}