#include "TrailSystem.h"
#include "../../Manager/Texture/TextureManager.h"

void TrailSystem::Update(EntityManager& entityManager, float deltaTime)
{
	std::vector<Entity*> trailsToAdd;

	for (auto& entity : entityManager.GetEntitiesWithComponents<TrailComponent, TransformComponent>())
	{
		auto trail = entity->GetComponent<TrailComponent>();
		auto transform = entity->GetComponent<TransformComponent>();

		trail->elapsedTime += deltaTime;
		if (trail->elapsedTime >= trail->spawnInterval)
		{
			trail->elapsedTime = 0.0f;

			trailsToAdd.push_back(entity);
		}
	}

	for (auto& entity : trailsToAdd)
	{
		CreateTrailProjectile(entityManager, entity);
	}
}

void TrailSystem::LoadTextures()
{
	mFrames = TextureManager::GetTextures("assets/weapon/Fireball.png", 64, 64);
}

void TrailSystem::CreateTrailProjectile(EntityManager& em, Entity* entity)
{
	auto trail = entity->GetComponent<TrailComponent>();
	auto transform = entity->GetComponent<TransformComponent>();

	auto& trailEffect = em.CreateEntity(EntityType::Projectile);
	trailEffect.AddComponent<TransformComponent>(transform->GetPosition());
	trailEffect.AddComponent<DrawableComponent>(mFrames[0]);

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(20, 20));
	collisionShape->setOrigin(10, 10);
	trailEffect.AddComponent<CollisionComponent>(std::move(collisionShape));

	trailEffect.AddComponent<LifetimeComponent>(0.5f);
	trailEffect.AddComponent<DamageComponent>(trail->damage, 0.5f, Enemy);
}
