#include "RenderSystem.h"

void RenderSystem::Render(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<TransformComponent, DrawableComponent>())
	{
		auto transform = entity->GetComponent<TransformComponent>();
		auto drawable = entity->GetComponent<DrawableComponent>();

		auto animation = entity->GetComponent<AnimationComponent>();
		auto rotation = entity->GetComponent<RotationComponent>();
 
		auto collision = entity->GetComponent<CollisionComponent>();

		if (collision)
		{
			auto& shape = collision->shape;
			shape->setOutlineColor(sf::Color::Green);
			shape->setOutlineThickness(1.0f);
			shape->setFillColor(sf::Color(0, 0, 0, 0)); // Прозрачный
			mWindow.draw(*shape);
		}

		drawable->sprite.setPosition(transform->x, transform->y);

		if (rotation)
		{
			drawable->sprite.setRotation(rotation->angle);
		}
		else
		{
			float currentScaleX = drawable->sprite.getScale().x;
			float newScaleX = std::copysign(1.0f, transform->lastDirection.x) * std::abs(currentScaleX);

			if (currentScaleX != newScaleX)
			{
				drawable->sprite.setScale(newScaleX, drawable->sprite.getScale().y);
			}
		}
		
		if (animation && !animation->isAnimating && !animation->frames.empty())
		{
			drawable->sprite.setTexture(animation->frames[0]);
		}

		mWindow.draw(drawable->sprite);

		if (animation && animation->isAnimating)
		{
			animation->elapsedTime += deltaTime;

			RenderAnimatedEntity(drawable, animation);

			drawable->sprite.setTexture(animation->frames[animation->currentFrameIndex]);
		}
	}
}

void RenderSystem::RenderAnimatedEntity(DrawableComponent* drawable, AnimationComponent* animation)
{
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
		}
	}
}
