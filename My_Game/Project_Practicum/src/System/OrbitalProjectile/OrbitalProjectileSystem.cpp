#include "OrbitalProjectileSystem.h"

void OrbitalProjectileSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<TransformComponent, OrbitalComponent, LifetimeComponent>())
	{
		auto transform = entity->GetComponent<TransformComponent>();
		auto orbitComponent = entity->GetComponent<OrbitalComponent>();

		orbitComponent->angle += orbitComponent->orbitSpeed * deltaTime;

		float newX = orbitComponent->parentTransform->x + orbitComponent->orbitRadius * cos(orbitComponent->angle);
		float newY = orbitComponent->parentTransform->y + orbitComponent->orbitRadius * sin(orbitComponent->angle);

		transform->x = newX;
		transform->y = newY;
	}
}