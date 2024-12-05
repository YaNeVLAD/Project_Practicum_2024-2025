#include "DamageSystem.h"

void DamageSystem::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto& entity : entityManager.GetEntitiesWithComponents<DamageComponent>())
	{
		auto damage = entity->GetComponent<DamageComponent>();

		damage->UpdateCooldown(deltaTime);
	}
}