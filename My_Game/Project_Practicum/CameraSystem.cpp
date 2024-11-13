#include "CameraSystem.h"

void CameraSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		auto camera = entity.GetComponent<CameraComponent>();
		auto transform = entity.GetComponent<TransformComponent>();

		if (camera && transform)
		{
			mView.setCenter(transform->x, transform->y);
			mWindow.setView(mView);
			break;
		}
	}
}
