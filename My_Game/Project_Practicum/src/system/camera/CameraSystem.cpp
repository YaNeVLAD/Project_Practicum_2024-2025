#include "CameraSystem.h"

void CameraSystem::Render(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<CameraComponent, TransformComponent>())
	{
		auto camera = entity->GetComponent<CameraComponent>();
		auto transform = entity->GetComponent<TransformComponent>();

		sf::Vector2f targetPosition = { transform->x, transform->y };

		mView.setCenter(mView.getCenter() + (targetPosition - mView.getCenter()));

		break;
	}
}