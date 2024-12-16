#pragma once

#include "../System.h"
#include "../../../ui/Screen/Screen.h"
#include "../../../ui/ProgressBar/ProgressBar.h"

class HUDSystem : public IRenderSystem
{
public:
	HUDSystem(sf::RenderWindow& window, sf::View& camera) 
		: mWindow(window), mCamera(camera)
	{
		LoadFont();
	}

	void Render(EntityManager& entityManager, float deltaTime) override;

private:
	Screen mScreen;

	ProgressBar mXpBar;
	ProgressBar mCooldown;
	ProgressBar mPlayerHealth;

	sf::Font mFont;
	sf::View& mCamera;
	sf::RenderWindow& mWindow;

	void RenderHealth(Entity* entity);

	void RenderXPBar(Entity* player);
	void RenderAbility(Entity* player);
	void RenderPlayerHealth(Entity* player);

	void LoadFont();
};

