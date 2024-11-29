#pragma once

#include "../System.h"
#include "../../../ui/Screen/Screen.h"

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

	sf::Font mFont;
	sf::View& mCamera;
	sf::RenderWindow& mWindow;

	void RenderHealth(Entity* entity);
	void RenderPlayerHealth(Entity* player);
	void RenderXPBar(Entity* player);

	void LoadFont();
};

