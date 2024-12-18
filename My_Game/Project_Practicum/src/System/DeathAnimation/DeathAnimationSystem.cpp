#include "DeathAnimationSystem.h"

void DeathAnimationSystem::Render(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<AnimationComponent, DeathAnimationComponent>())
	{
		auto deathAnimation = entity->GetComponent<DeathAnimationComponent>();
		auto playerHealth = entity->GetComponent<PlayerHealthComponent>();

		if (!deathAnimation->isCompleted)
		{
			if (mNeedToChangeCamera || playerHealth != nullptr) {
				ChangeCameraTarget(entityManager);
			}
			StartAnimation(entity);
			if (mNeedToChangeCamera || playerHealth != nullptr)
			{
				ZoomIn(entity, deltaTime);
				ZoomOut(entity);
				RestoreCameraTarget(entityManager);
			}
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
	auto players = em.GetEntitiesWithType(Player);

	for (auto& player : players)
	{
		player->RemoveComponent<CameraComponent>();
	}

	for (auto& dyingEntity : em.GetEntitiesWithComponents<DeathAnimationComponent>())
	{
		dyingEntity->AddComponent<CameraComponent>();
	}
}

void DeathAnimationSystem::RestoreCameraTarget(EntityManager& em)
{
	auto entities = em.GetEntitiesWithComponents<DeathAnimationComponent>();
	if (!entities.empty())
	{
		return;
	}

	for (auto& entity : em.GetEntitiesWithComponents<CameraComponent>())
	{
		entity->RemoveComponent<CameraComponent>();
	}
	for (auto& player : em.GetEntitiesWithType(Player))
	{
		player->AddComponent<CameraComponent>();
	}
}

void DeathAnimationSystem::StartAnimation(Entity* entity)
{
	auto animation = entity->GetComponent<AnimationComponent>();
	auto deathAnimation = entity->GetComponent<DeathAnimationComponent>();
	auto transform = entity->GetComponent<TransformComponent>();

	auto playerHealth = entity->GetComponent<PlayerHealthComponent>();

	if (animation->currentFrameIndex != 0 || deathAnimation->elapsedTime != 0.f)
	{
		return;
	}

	if (transform)
	{
		transform->multiplier = { 0, 0 };
	}
	entity->RemoveComponent<CollisionComponent>();

	if (mNeedToChangeCamera || playerHealth != nullptr)
	{
		mIsPaused = true;
	}
	deathAnimation->currentZoom = INITIAL_ZOOM;
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
		auto playerHealth = entity->GetComponent<PlayerHealthComponent>();
		auto bossHealth = entity->GetComponent<BossHealthComponent>();

		if (mNeedToChangeCamera || playerHealth != nullptr)
		{
			mIsPaused = false;
		}
		deathAnimation->isCompleted = true;

		(playerHealth == nullptr)
			? entity->AddComponent<VictoryComponent>()
			: entity->AddComponent<GameOverComponent>();
	}
}