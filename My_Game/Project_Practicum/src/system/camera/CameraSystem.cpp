#include "CameraSystem.h"

void CameraSystem::Render(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<CameraComponent, TransformComponent>())
	{
		auto camera = entity->GetComponent<CameraComponent>();
		auto transform = entity->GetComponent<TransformComponent>();

		mView.setCenter(transform->x, transform->y);
		break;
	}
}