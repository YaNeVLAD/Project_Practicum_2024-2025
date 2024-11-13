#include "RenderSystem.h"
#include "System.h"

void RenderSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		auto position = entity.GetComponent<PositionComponent>();
		auto drawable = entity.GetComponent<DrawableComponent>();

		if (position && drawable)
		{
			drawable->shape.setPosition(position->x, position->y);
			mWindow.draw(drawable->shape);
		}
	}
}