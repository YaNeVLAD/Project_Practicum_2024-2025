#pragma once

#include "../System.h"

class RenderSystem : public System
{
public:
	RenderSystem(sf::RenderWindow& window) : mWindow(window) {}

	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	void RenderAnimatedEntity(DrawableComponent* drawable, AnimationComponent* animation);
	sf::RenderWindow& mWindow;
};