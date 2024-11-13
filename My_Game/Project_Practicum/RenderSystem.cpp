#include "RenderSystem.h"
#include "System.h"

void RenderSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		auto transform = entity.GetComponent<TransformComponent>();
		auto drawable = entity.GetComponent<DrawableComponent>();

		if (transform && drawable)
		{
			drawable->shape.setPosition(transform->x, transform->y);
			mWindow.draw(drawable->shape);
		}
	}
}