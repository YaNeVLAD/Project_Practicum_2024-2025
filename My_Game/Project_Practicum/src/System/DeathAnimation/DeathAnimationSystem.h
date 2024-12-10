#pragma once

#include "../System.h"

class DeathAnimationSystem : public IRenderSystem
{
public:
	DeathAnimationSystem(sf::View& camera, bool& isPaused) : mCamera(camera), mIsPaused(isPaused) {}
	void Render(EntityManager& entityManager, float deltaTime) override;

private:
	const float INITIAL_ZOOM = 1.f;
	const float TARGET_ZOOM = 0.75f;
	const float ZOOM_DURATION = 0.5f;

	bool mIsZoomed = false;
	bool& mIsPaused;

	sf::View& mCamera;
};