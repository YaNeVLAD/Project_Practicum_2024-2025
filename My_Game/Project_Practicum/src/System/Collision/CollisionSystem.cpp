#include "CollisionSystem.h"

#include <iostream>
#include <set>

void CollisionSystem::Update(EntityManager& entityManager, float deltaTime)
{
	if (mEntityManager == nullptr)
	{
		mEntityManager = &entityManager;
	}
	auto entities = entityManager.GetEntitiesWithComponents<CollisionComponent>();

	if (entities.empty())
	{
		return;
	}

	std::vector<CollisionEvent> events;

	for (auto& entity : entities)
	{
		auto collision = entity->GetComponent<CollisionComponent>();
		sf::FloatRect rect = collision->getRect();

		events.push_back(CollisionEvent(rect.left, CollisionEvent::Type::Open, entity, rect));
		events.push_back(CollisionEvent(rect.left + rect.width, CollisionEvent::Type::Close, entity, rect));
	}

	std::sort(events.begin(), events.end());

	std::set<std::pair<float, Entity*>> active;
	std::vector<std::pair<Entity*, Entity*>> collidedEntities;

	for (const auto& event : events)
	{
		if (event.type == CollisionEvent::Type::Open)
		{
			for (const auto& activeRect : active)
			{
				const auto& otherEntity = activeRect.second;

				auto otherCollision = otherEntity->GetComponent<CollisionComponent>();
				sf::FloatRect otherRect = otherCollision->getRect();

				if (!event.rect.intersects(otherRect))
				{
					continue;
				}

				collidedEntities.push_back({ event.entity, otherEntity });
			}
			active.insert(std::pair(event.rect.top, event.entity));
		}
		else
		{
			active.erase(std::pair(event.rect.top, event.entity));
		}
	}

	for (auto& entityPair : collidedEntities)
	{
		HandleCollision(entityPair.first, entityPair.second);
	}

	for (auto* entity : mCollidedDamageEntities)
	{
		auto damage = entity->GetComponent<DamageComponent>();
		if (damage != nullptr && damage->CanDealDamage())
		{
			damage->StartCooldown();
		}
	}
	mCollidedDamageEntities.clear();
}

void CollisionSystem::ApplyBonus(Entity* player, Entity* bonus)
{
	if (player == nullptr || bonus == nullptr)
	{
		return;
	}

	auto bonusComponent = bonus->GetComponent<BonusComponent>();
	auto lifetime = bonus->GetComponent<LifetimeComponent>();

	if (bonusComponent == nullptr || lifetime == nullptr)
	{
		return;
	}

	lifetime->time = -1.f;

	if (bonusComponent->type == BonusComponent::BonusType::Health)
	{
		auto health = player->GetComponent<PlayerHealthComponent>();
		if (health == nullptr)
		{
			return;
		}

		health->AddHealth(50);
	}
	else if (bonusComponent->type == BonusComponent::BonusType::Bomb)
	{
		auto damage = bonus->GetComponent<DamageComponent>();

		if (damage == nullptr)
		{
			return;
		}

		for (auto& enemy : mEntityManager->GetEntitiesWithType(EntityType::Enemy))
		{
			auto health = enemy->GetComponent<HealthComponent>();
			if (health == nullptr)
			{
				continue;
			}
			damage->DealDamage(health);
		}
	}
	else if (bonusComponent->type == BonusComponent::BonusType::Magnet)
	{
		for (auto& experience : mEntityManager->GetEntitiesWithComponents<ExperienceComponent>())
		{
			experience->AddComponent<HomingProjectileComponent>(1000.f, EntityType::Player);
		}
	}
}

void CollisionSystem::DealDamage(Entity* target, DamageComponent* damage)
{
	if (damage == nullptr || target == nullptr)
	{
		return;
	}

	if (target->GetType() != damage->targetType)
	{
		return;
	}

	auto health = target->GetComponent<HealthComponent>();
	auto bossHealth = target->GetComponent<BossHealthComponent>();
	auto playerHealth = target->GetComponent<PlayerHealthComponent>();

	auto container = target->GetComponent<ContainerComponent>();

	auto animation = target->GetComponent<AnimationComponent>();

	if (animation != nullptr)
	{
		animation->SetState(AnimationComponent::HURT);
	}

	if (container != nullptr)
	{
		container->isDestroyed = true;
	}
	if (health != nullptr)
	{
		damage->DealDamage(health);
	}
	if (playerHealth != nullptr)
	{
		damage->DealDamage(playerHealth);
	}
	if (bossHealth != nullptr)
	{
		damage->DealDamage(bossHealth);
	}
}

void CollisionSystem::HandleCollision(Entity* first, Entity* second)
{
	if (first == nullptr || second == nullptr)
	{
		return;
	}

	auto firstType = first->GetType();
	auto secondType = second->GetType();

	if ((firstType & Player && secondType & Enemy) || (firstType & Enemy && secondType & Player))
	{
		auto enemy = (firstType & Enemy) ? first : second;
		auto player = (firstType & Player) ? first : second;
		DealDamage(player, enemy->GetComponent<DamageComponent>());

		mCollidedDamageEntities.insert(enemy);
	}
	else if ((firstType & Player && secondType & Particle) || (firstType & Particle && secondType & Player))
	{
		auto player = (firstType & Player) ? first : second;
		auto particle = (firstType & Particle) ? first : second;
		ApplyParticleEffect(player, particle);
	}
	else if ((firstType & Bonus && secondType & Player) || (firstType & Player && secondType & Bonus))
	{
		auto player = (firstType & Player) ? first : second;
		auto bonus = (firstType & Bonus) ? first : second;
		ApplyBonus(player, bonus);
	}
	else if (firstType & Projectile || secondType & Projectile)
	{
		auto projectile = (firstType & Projectile) ? first : second;
		auto target = (firstType & Projectile) ? second : first;
		DealDamage(target, projectile->GetComponent<DamageComponent>());

		mCollidedDamageEntities.insert(projectile);
	}
	else if (firstType & Enemy && secondType & Enemy)
	{
		HandlePushAway(first, second);
	}
}

void CollisionSystem::ApplyParticleEffect(Entity* player, Entity* particle)
{
	if (player == nullptr || particle == nullptr)
	{
		return;
	}

	auto lvl = player->GetComponent<LevelComponent>();
	auto exp = particle->GetComponent<ExperienceComponent>();
	auto lifetime = particle->GetComponent<LifetimeComponent>();

	if (lvl != nullptr && exp != nullptr)
	{
		lvl->GainExperience(exp->amount);
	}

	if (lifetime != nullptr)
	{
		lifetime->time = 0.f;
	}
}

void CollisionSystem::HandlePushAway(Entity* first, Entity* second)
{
	auto firstTransform = first->GetComponent<TransformComponent>();
	auto secondTransform = second->GetComponent<TransformComponent>();

	sf::Vector2f direction = sf::Vector2f(firstTransform->x, firstTransform->y) - sf::Vector2f(secondTransform->x, secondTransform->y);
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance < 1.0f) return;

	direction /= distance;

	float pushStrength = 2.0f;
	float pushX = direction.x * pushStrength;
	float pushY = direction.y * pushStrength;

	firstTransform->x += pushX;
	firstTransform->y += pushY;
}