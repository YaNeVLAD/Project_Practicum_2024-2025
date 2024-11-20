#include "InputSystem.h"

void InputSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		auto input = entity.GetComponent<InputComponent>();
		auto velocity = entity.GetComponent<VelocityComponent>();

		if (input && velocity)
		{
			velocity->vx = 0;
			velocity->vy = 0;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				velocity->vy = -100;  // �������� �����
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				velocity->vy = 100;   // �������� ����
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				velocity->vx = -100;  // �������� �����
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				velocity->vx = 100;   // �������� ������
			}
		}
	}
}
