#include "TrailSystem.h"
#include "../../Manager/Texture/TextureManager.h"
#include "../../Config/GameConfig.h"

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

void TrailSystem::CreateTrailProjectile(EntityManager& em, Entity* entity) const
{
	auto trail = entity->GetComponent<TrailComponent>();
	auto transform = entity->GetComponent<TransformComponent>();

	auto& trailEffect = em.CreateEntity(EntityType::Projectile);
	trailEffect.AddComponent<TransformComponent>(transform->GetPosition());
	trailEffect.AddComponent<AnimationComponent>(0.2f, true);
	auto animation = trailEffect.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::ATTACK, mFrames);
	animation->SetState(AnimationComponent::ATTACK);

	trailEffect.AddComponent<DrawableComponent>(mFrames->at(0));

	auto collisionShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(20, 20));
	collisionShape->setOrigin(10, 10);
	trailEffect.AddComponent<CollisionComponent>(std::move(collisionShape));

	trailEffect.AddComponent<LifetimeComponent>(2.f);
	trailEffect.AddComponent<DamageComponent>(
		trail->damage + GameConfig::GetInstance()->playerDamageBuff + (GameConfig::GetInstance()->weaponStats[GameConfig::GetInstance()->Fireball] / 2), 0.1f, Enemy
	);
}