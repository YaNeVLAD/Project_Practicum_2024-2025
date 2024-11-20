#include "App.h"

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

		if (mCurrentState == App::MainMenu) {
			HandleMainMenuEvents(event);
		}
		else if (mCurrentState == App::Playing) {
			mGame.ProcessEvents();
		}
		else if (mCurrentState == App::Defeat) {
			HandleDefeatScreenEvents(event);
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
	case App::Defeat:
		RenderDefeatScreen();
		break;
	}

	mWindow.display();
}

void App::HandleMainMenuEvents(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		mCurrentState = App::Playing;
		mGame.init();
	}
}

void App::HandleDefeatScreenEvents(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);

		if (mExitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
		{
			mWindow.close();
		}
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