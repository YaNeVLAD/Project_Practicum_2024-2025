#include "HUDSystem.h"

#include "../../Manager/Entity/EntityManager.h"

void HUDSystem::Render(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		RenderHealth(entityManager);
		RenderPlayerHealth(entityManager);
		RenderXPBar(entityManager);
	}
}

void HUDSystem::RenderHealth(EntityManager& em)
{
	for (auto& entity : em.GetEntitiesWithComponents<HealthComponent, TransformComponent>())
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

		if (static_cast<int>(healthProgress) < 1)
		{
			mWindow.draw(healthBarBg);
			mWindow.draw(healthBar);
		}
	}
}

void HUDSystem::RenderPlayerHealth(EntityManager& em)
{
	auto player = em.GetEntitiesWithComponents<PlayerHealthComponent, TransformComponent>();

	if (player.empty())
	{
		return;
	}

	auto transform = player.front()->GetComponent<TransformComponent>();
	auto health = player.front()->GetComponent<PlayerHealthComponent>();

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

	mWindow.draw(healthBarBg);
	mWindow.draw(healthBar);
}

void HUDSystem::RenderXPBar(EntityManager& em)
{
	sf::Vector2f cameraCenter = mCamera.getCenter();
	sf::Vector2f cameraSize = mCamera.getSize();

	float barWidth = 800.0f;
	float barHeight = 30.0f;
	float barX = cameraCenter.x - (barWidth / 2.0f);
	float barY = cameraCenter.y - (cameraSize.y / 2.0f) + 20.0f;

	for (auto& entity : em.GetEntitiesWithComponents<ExperienceComponent>())
	{
		auto experience = entity->GetComponent<ExperienceComponent>();

		sf::RectangleShape background(sf::Vector2f(barWidth, barHeight));
		background.setPosition(barX, barY);
		background.setFillColor(sf::Color(50, 50, 50));

		float fillRatio = static_cast<float>(experience->currentExperience) / experience->maxExperience;
		sf::RectangleShape fill(sf::Vector2f(barWidth * fillRatio, barHeight));
		fill.setPosition(barX, barY);
		fill.setFillColor(sf::Color(100, 200, 100));

		sf::Font font;
		if (!font.loadFromFile("assets/font/Roboto-Bold.ttf"))
		{
			// TODO: Добавить обработку ошибки загрузки шрифта
		}
		sf::Text levelText("Lv " + std::to_string(experience->level), font, 20);
		levelText.setPosition(barX - 100.0f, barY);
		levelText.setFillColor(sf::Color::White);

		mWindow.draw(background);
		mWindow.draw(fill);
		mWindow.draw(levelText);
	}
}