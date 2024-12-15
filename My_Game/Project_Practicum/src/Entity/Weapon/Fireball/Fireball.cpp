#include "Fireball.h"

#include "../../../Manager/Texture/TextureManager.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"

void Fireball::Upgrade(int level)
{
	if (CanUpgrade())
	{
		mLevel++;
		damage += 10;
		mTrailDamage += 5;
		projectileSpeed += 50;
		mScale += { 0.2f, 0.2f };
	}
}

void Fireball::Attack(EntityManager& entityManager, TransformComponent* parentTransform, TransformComponent* playerTransform)
{
	auto& fireball = entityManager.CreateEntity(Projectile);

	fireball.AddComponent<TransformComponent>(
		playerTransform->GetPosition(),
		projectileSpeed * playerTransform->lastDirection
	);

	fireball.AddComponent<DrawableComponent>(mFrames[0], mScale);
	fireball.AddComponent<AnimationComponent>(0.3f, true);
	auto animation = fireball.GetComponent<AnimationComponent>();
	animation->AddAnimation(AnimationComponent::ATTACK, mFrames);
	animation->SetState(AnimationComponent::ATTACK);

	fireball.AddComponent<LifetimeComponent>(1.8f);

	auto collisionShape = std::make_unique<sf::RectangleShape>(
		sf::Vector2f(32.0f * mScale.x, 32.0f * mScale.y)
	);
	collisionShape->setOrigin(
		16.0f * mScale.x, 16.0f * mScale.y
	);

	fireball.AddComponent<CollisionComponent>(std::move(collisionShape));

	fireball.AddComponent<DamageComponent>(damage, 0.5f, Enemy);

	fireball.AddComponent<TrailComponent>(mTrailDamage, 0.15f);
}

void Fireball::LoadTextures()
{
	mFrames = TextureManager::GetTextures("assets/weapon/Fireball.png", 64, 64);
}
