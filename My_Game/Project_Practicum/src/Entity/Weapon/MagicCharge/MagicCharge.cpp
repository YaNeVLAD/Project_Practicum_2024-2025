#include "MagicCharge.h"
#include "../../../Manager/Entity/EntityManager.h"
#include "../../../component/Components.h"
#include "../../../../utils/SpriteSheet.h"

void MagicCharge::Attack(EntityManager& entityManager, TransformComponent* parentTransform)
{
	Entity& projectile = entityManager.CreateEntity();
	projectile.AddComponent<TransformComponent>(
		parentTransform->x + 16, parentTransform->y + 32,
		parentTransform->lastDirection.x * projectileSpeed,
		parentTransform->lastDirection.y * projectileSpeed
	);
	projectile.AddComponent<ProjectileComponent>(3.0f);
	projectile.AddComponent<DrawableComponent>(mFrames[0], sf::Vector2f(1.5, 1.5));
	projectile.AddComponent<AnimationComponent>(mFrames, 0.2f, true, -1.0f, true);
}

void MagicCharge::LoadTextures()
{
	mFrames = SpriteSheet::LoadTextures("assets/character/Charge.png", 64, 64);
}