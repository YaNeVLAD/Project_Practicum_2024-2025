#pragma once

namespace Systems
{
	class RenderSystem;
}

class RenderSystem : public System
{
public:
	RenderSystem(sf::RenderWindow& window) : mWindow(window) {}

	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	sf::RenderWindow& mWindow;
};