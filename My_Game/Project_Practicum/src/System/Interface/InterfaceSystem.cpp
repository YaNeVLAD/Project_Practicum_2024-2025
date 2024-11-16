#include "InterfaceSystem.h"

#include "../../Manager/Entity/EntityManager.h"

void InterfaceSystem::Render(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<HealthComponent, TransformComponent>())
	{
		auto health = entity->GetComponent<HealthComponent>();
		auto transform = entity->GetComponent<TransformComponent>();

		sf::RectangleShape healthBar(sf::Vector2f(50, 10));
		healthBar.setOrigin(25, 5);
		sf::RectangleShape healthBarBg(sf::Vector2f(50, 10));
		healthBarBg.setOrigin(25, 5);
		healthBarBg.setPosition(
			transform->x,
			transform->y + 50
		);
		healthBar.setPosition(
			transform->x,
			transform->y + 50
		);
		healthBar.setFillColor(sf::Color::Green);
		healthBarBg.setFillColor(sf::Color::Red);

		float healthProgress = static_cast<float>(health->currentHealth) / health->maxHealth;
		healthBar.setSize(sf::Vector2f(50 * healthProgress, 10));

		if (healthProgress != 1)
		{
			mWindow.draw(healthBarBg);
			mWindow.draw(healthBar);
		}
	}
}
