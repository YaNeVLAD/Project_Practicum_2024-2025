#include "CameraSystem.h"

void CameraSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		auto camera = entity.GetComponent<CameraComponent>();
		auto position = entity.GetComponent<PositionComponent>();

		if (camera && position)
		{
			mView.setCenter(position->x, position->y);
			mWindow.setView(mView);
			break;
		}
	}
}
