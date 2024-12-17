#include "DeathAnimationSystem.h"

void DeathAnimationSystem::Render(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<AnimationComponent, DeathAnimationComponent>())
	{
		auto deathAnimation = entity->GetComponent<DeathAnimationComponent>();
		auto animation = entity->GetComponent<AnimationComponent>();

		if (!deathAnimation->isCompleted)
		{
			ChangeCameraTarget(entityManager);
			StartAnimation(entity);
			ZoomIn(entity, deltaTime);
			ZoomOut(entity);
			RestoreCameraTarget(entityManager);
			EndAnimation(entity);
		}
	}
}

void DeathAnimationSystem::RestoreInitialZoom()
{
	mCamera.setSize(mInitialSize);
}

void DeathAnimationSystem::ChangeCameraTarget(EntityManager& em)
{
	auto targets = em.GetEntitiesWithComponents<CameraComponent>();

	for (auto& target : targets)
	{
		target->RemoveComponent<CameraComponent>();
	}

	auto entities = em.GetEntitiesWithComponents<DeathAnimationComponent>();

	if (entities.empty())
	{
		return;
	}

	entities.front()->AddComponent<CameraComponent>();
}

void DeathAnimationSystem::RestoreCameraTarget(EntityManager& em)
{
	auto player = em.GetEntitiesWithType(Player);
	if (player.empty())
	{
		return;
	}
	for (auto& entity : em.GetEntitiesWithComponents<CameraComponent>())
	{
		entity->RemoveComponent<CameraComponent>();
	}
	player.front()->AddComponent<CameraComponent>();
}

void DeathAnimationSystem::StartAnimation(Entity* entity)
{
	auto animation = entity->GetComponent<AnimationComponent>();
	auto deathAnimation = entity->GetComponent<DeathAnimationComponent>();

	if (animation->currentFrameIndex == 0 && deathAnimation->elapsedTime == 0.0f)
	{
		mIsPaused = true;
		deathAnimation->currentZoom = INITIAL_ZOOM;
	}
}

void DeathAnimationSystem::ZoomIn(Entity* entity, float dt)
{
	auto deathAnimation = entity->GetComponent<DeathAnimationComponent>();

	float zoomStep = (TARGET_ZOOM - INITIAL_ZOOM) / (ZOOM_DURATION / dt);

	if (deathAnimation->elapsedTime < ZOOM_DURATION)
	{
		mCamera.zoom(INITIAL_ZOOM + zoomStep);
		deathAnimation->elapsedTime += dt;
		deathAnimation->currentZoom += zoomStep;
	}
}

void DeathAnimationSystem::ZoomOut(Entity* entity)
{
	auto animation = entity->GetComponent<AnimationComponent>();
	auto deathAnimation = entity->GetComponent<DeathAnimationComponent>();

	if (animation->currentFrameIndex == animation->animations[AnimationComponent::DEAD].size() - 1)
	{
		RestoreInitialZoom();
		animation->frameTime = 0.0f;
	}
}

void DeathAnimationSystem::EndAnimation(Entity* entity)
{
	auto animation = entity->GetComponent<AnimationComponent>();
	auto deathAnimation = entity->GetComponent<DeathAnimationComponent>();

	if (animation->currentFrameIndex == animation->animations[AnimationComponent::DEAD].size())
	{
		mIsPaused = false;
		deathAnimation->isCompleted = true;

		auto playerHealth = entity->GetComponent<PlayerHealthComponent>();
		auto bossHealth = entity->GetComponent<BossHealthComponent>();

		(playerHealth == nullptr)
			? entity->AddComponent<VictoryComponent>()
			: entity->AddComponent<GameOverComponent>();
	}
}