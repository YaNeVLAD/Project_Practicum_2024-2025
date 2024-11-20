#include "InputSystem.h"

void InputSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<InputComponent, TransformComponent>())
	{
		auto input = entity->GetComponent<InputComponent>();
		auto transform = entity->GetComponent<TransformComponent>();
		auto animation = entity->GetComponent<AnimationComponent>();

		transform->vx = 0;
		transform->vy = 0;

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

		if (animation)
		{
			animation->isAnimating = (transform->vx != 0 || transform->vy != 0);
		}
	}
}