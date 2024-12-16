#pragma once

#include "../System.h"

class InputSystem : public IUpdateSystem
{
public:
	void Update(EntityManager& entityManager, float deltaTime) override;

private:
	void ApplyAbility(Entity* entity, AbilityComponent* ability, float deltaTime);
	void ActivateAbility(Entity* entity, AbilityComponent* ability);
	void UpdateAbilityEffects(Entity* entity, AbilityComponent* ability);
};