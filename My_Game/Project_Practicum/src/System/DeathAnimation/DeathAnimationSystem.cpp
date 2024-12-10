#include "DeathAnimationSystem.h"

void DeathAnimationSystem::Render(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<AnimationComponent, DeathAnimationComponent>())
	{
		auto deathAnimation = entity->GetComponent<DeathAnimationComponent>();
		auto animation = entity->GetComponent<AnimationComponent>();

		if (!deathAnimation->isCompleted)
		{
			if (animation->currentFrameIndex == 0 && deathAnimation->elapsedTime == 0.0f)
			{
				mIsPaused = true;
				mIsZoomed = true;
				deathAnimation->currentZoom = INITIAL_ZOOM;
			}

			float zoomStep = (TARGET_ZOOM - INITIAL_ZOOM) / (ZOOM_DURATION / deltaTime);

			if (deathAnimation->elapsedTime < ZOOM_DURATION)
			{
				mCamera.zoom(1.0f + zoomStep);
				deathAnimation->currentZoom += zoomStep;
				deathAnimation->elapsedTime += deltaTime;
			}

			if (animation->currentFrameIndex == animation->animations[AnimationComponent::DEAD].size() - 1 && mIsZoomed)
			{
				mCamera.zoom(1.0f / deathAnimation->currentZoom);
				animation->frameTime = 0.0f;
				mIsZoomed = false;
			}

			if (animation->currentFrameIndex == animation->animations[AnimationComponent::DEAD].size())
			{
				mIsPaused = false;
				deathAnimation->isCompleted = true;
				entity->AddComponent<GameOverComponent>();
			}
		}
	}
}