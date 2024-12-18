#include "InputSystem.h"

void InputSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<InputComponent, TransformComponent>())
	{
		auto input = entity->GetComponent<InputComponent>();
		auto transform = entity->GetComponent<TransformComponent>();
		auto animation = entity->GetComponent<AnimationComponent>();
		auto ability = entity->GetComponent<AbilityComponent>();

		transform->vx = 0;
		transform->vy = 0;

		if (ability != nullptr)
		{
			ApplyAbility(entity, ability, deltaTime);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			transform->vy = -200;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			transform->vy = 200;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			transform->vx = -200;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			transform->vx = 200;
		}
	}
}

void InputSystem::ApplyAbility(Entity* entity, AbilityComponent* ability, float deltaTime)
{
	if (entity == nullptr || ability == nullptr)
	{
		return;
	}

	ability->UpdateTimer(deltaTime);

	if (ability->CanBeUsed() && sf::Keyboard::isKeyPressed(ability->activationKey))
	{
		ActivateAbility(entity, ability);
	}

	UpdateAbilityEffects(entity, ability);
}

void InputSystem::ActivateAbility(Entity* entity, AbilityComponent* ability)
{
	auto health = entity->GetComponent<PlayerHealthComponent>();
	switch (ability->type)
	{
	case AbilityComponent::Heal:
		if (health != nullptr)
		{
			health->AddHealth(30);
		}
		break;
	case AbilityComponent::Haste:

		break;
	default:
		break;
	}
	
	ability->Activate();
}

void InputSystem::UpdateAbilityEffects(Entity* entity, AbilityComponent* ability)
{
	auto transform = entity->GetComponent<TransformComponent>();
	if (ability->type == AbilityComponent::Haste && transform != nullptr)
	{
		transform->multiplier = ability->IsActive() 
			? sf::Vector2f(2.f, 2.f) 
			: sf::Vector2f(1, 1);
	}
}