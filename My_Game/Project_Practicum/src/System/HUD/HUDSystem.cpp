#include "HUDSystem.h"

#include "../../Manager/Entity/EntityManager.h"
#include "../../Manager/Texture/TextureManager.h"
#include "../../../ui/ProgressBar/ProgressBar.h"

void HUDSystem::Render(EntityManager& entityManager, float deltaTime)
{
	mScreen.Clear();

	auto player = entityManager.GetEntitiesWithComponents<PlayerHealthComponent, TransformComponent>();

	if (player.empty())
	{
		return;
	}

	RenderXPBar(player.front());
	RenderPlayerHealth(player.front());

	for (auto& entity : entityManager.GetEntities())
	{
		RenderHealth(&entity);
	}

	mWindow.draw(mScreen);
}

void HUDSystem::RenderHealth(Entity* entity)
{
	auto health = entity->GetComponent<HealthComponent>();
	auto transform = entity->GetComponent<TransformComponent>();

	if (health == nullptr || transform == nullptr)
	{
		return;
	}

	float healthProgress = static_cast<float>(health->currentHealth) / health->maxHealth;

	if (static_cast<int>(healthProgress) >= 1)
	{
		return;
	}

	sf::Vector2f position = { transform->x - 25.f, transform->y + 50.f };

	ProgressBar healthBar;
	healthBar
		.SetSize({ 50.f, 10.f })
		.SetProgress(healthProgress)
		.SetBackgroundColor(sf::Color::Red)
		.SetProgressLineColor(sf::Color::Green)
		.SetPosition(View::Alignment::Default, mCamera, position);

	mScreen.AddView(std::make_shared<ProgressBar>(healthBar));
}

void HUDSystem::RenderPlayerHealth(Entity* player)
{
	auto health = player->GetComponent<PlayerHealthComponent>();
	auto transform = player->GetComponent<TransformComponent>();

	float healthProgress = static_cast<float>(health->currentHealth) / health->maxHealth;
	sf::Vector2f position = { transform->x - 30.f, transform->y + 77.5f };

	ProgressBar playerHealthBar;
	playerHealthBar
		.SetSize({ 60.f, 12.5f })
		.SetProgress(healthProgress)
		.SetBackgroundColor(sf::Color::Red)
		.SetProgressLineColor(sf::Color::Green)
		.SetPosition(View::Alignment::Default, mCamera, position);

	mScreen.AddView(std::make_shared<ProgressBar>(playerHealthBar));
}

void HUDSystem::RenderXPBar(Entity* player)
{
	auto experience = player->GetComponent<ExperienceComponent>();

	ProgressBar xpBar;
	xpBar
		.SetSize({ 800.f, 30.f })
		.SetBackgroundColor(sf::Color(50, 50, 50))
		.SetProgressLineColor(sf::Color(100, 200, 100))
		.SetPosition(View::Alignment::Center, mCamera, { 0.f, -(mCamera.getSize().y / 2.f) + 20.f })
		.SetProgress(static_cast<float>(experience->currentExperience) / experience->maxExperience);

	sf::Text levelText("Lv " + std::to_string(experience->level), mFont, 20);
	levelText.setPosition(xpBar.GetPosition().x - 100.f, xpBar.GetPosition().y);
	levelText.setFillColor(sf::Color::White);

	mScreen.AddView(std::make_shared<ProgressBar>(xpBar));
	mWindow.draw(levelText);
}

void HUDSystem::LoadFont()
{
	mFont = TextureManager::GetFont("assets/font/Roboto-Bold.ttf");
}