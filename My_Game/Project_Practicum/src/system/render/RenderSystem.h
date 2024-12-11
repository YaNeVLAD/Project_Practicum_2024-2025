#pragma once

#include "../System.h"

class RenderSystem : public IRenderSystem
{
public:
	RenderSystem(sf::RenderWindow& window) : mWindow(window) {}

	void Render(EntityManager& entityManager, float deltaTime) override;

private:
	void RenderAnimatedEntity(AnimationComponent* animation);
	sf::RenderWindow& mWindow;
};