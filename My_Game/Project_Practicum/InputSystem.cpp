#include "InputSystem.h"

void InputSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		auto input = entity.GetComponent<InputComponent>();
		auto transform = entity.GetComponent<TransformComponent>();

		if (input && transform)
		{
			transform->vx = 0;
			transform->vy = 0;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				transform->vy = -100;  // Движение вверх
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				transform->vy = 100;   // Движение вниз
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				transform->vx = -100;  // Движение влево
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				transform->vx = 100;   // Движение вправо
			}
		}
	}
}
