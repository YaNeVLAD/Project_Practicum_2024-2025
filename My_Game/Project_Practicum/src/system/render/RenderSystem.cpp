#include "RenderSystem.h"

void RenderSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntities())
	{
		auto transform = entity.GetComponent<TransformComponent>();
		auto drawable = entity.GetComponent<DrawableComponent>();
		auto animation = entity.GetComponent<AnimationComponent>();

		if (transform && drawable)
		{
			drawable->sprite.setPosition(transform->x, transform->y);

			if (animation && !animation->isAnimating && !animation->frames.empty())
			{
				drawable->sprite.setTexture(animation->frames[0]);
			}

			mWindow.draw(drawable->sprite);

			if (animation && animation->isAnimating)
			{
				animation->elapsedTime += deltaTime;

				if (animation->elapsedTime >= animation->frameTime)
				{
					animation->elapsedTime = 0.0f;
					animation->currentFrameIndex++;

					if (animation->currentFrameIndex >= animation->frames.size())
					{
						if (animation->loop)
						{
							animation->currentFrameIndex = 0;
						}
						else
						{
							entityManager.RemoveEntity(entity.GetId());
							continue;
						}
					}
				}

				drawable->sprite.setTexture(animation->frames[animation->currentFrameIndex]);
			}
		}
	}
}
