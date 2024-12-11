#pragma once

#include "../System.h"

class DeathAnimationSystem : public IRenderSystem
{
public:
	DeathAnimationSystem(sf::View& camera, bool& isPaused) 
		: mCamera(camera), mIsPaused(isPaused), mInitialSize(camera.getSize()) {}

	void Render(EntityManager& entityManager, float deltaTime) override;

private:
	sf::Vector2f mInitialSize;

	const float INITIAL_ZOOM = 1.f;
	const float TARGET_ZOOM = 0.75f;
	const float ZOOM_DURATION = 0.5f;

	bool& mIsPaused;

	sf::View& mCamera;

	void RestoreInitialZoom();
	void ChangeCameraTarget(EntityManager& em);

	void StartAnimation(Entity* entity);
	void ZoomIn(Entity* entity, float dt);
	void ZoomOut(Entity* entity);
	void EndAnimation(Entity* entity);
};