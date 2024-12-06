#include "ContainerSystem.h"

#include "../../Factory/Factory.h"

void ContainerSystem::Update(EntityManager& entityManager, float deltaTime)
{
	std::vector<Entity*> destroyedContainers;

	for (auto& entity : entityManager.GetEntitiesWithComponents<ContainerComponent>())
	{
		auto container = entity->GetComponent<ContainerComponent>();

		if (container->isDestroyed)
		{
			destroyedContainers.push_back(entity);
		}
	}

	for (auto& container : destroyedContainers)
	{
		auto transform = container->GetComponent<TransformComponent>();
		SpawnBonus(entityManager, sf::Vector2f(transform->x, transform->y));
		entityManager.RemoveEntity(container->GetId());
	}
}

void ContainerSystem::SpawnBonus(EntityManager& em, sf::Vector2f position)
{
	std::uniform_int_distribution<int> typeDistribution(
		BonusComponent::BonusType::Health,
		BonusComponent::BonusType::Magnet
	);

	int type = typeDistribution(mGenerator);

	switch (type)
	{
	case BonusComponent::BonusType::Health:
		Factory::CreateHealthBonus(em, position);
		break;
	case BonusComponent::BonusType::Bomb:
		Factory::CreateBombBonus(em, position);
		break;
	case BonusComponent::BonusType::Magnet:
		Factory::CreateMagnetBonus(em, position);
		break;
	default:
		break;
	}
}