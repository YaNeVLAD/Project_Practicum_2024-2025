#include "MagicCharge.h"
#include "../../../entity/Entity.h"
#include "../../../component/Components.h"
#include "../../../../utils/SpriteSheet.h"

void MagicCharge::Attack(Entity& projectile, const TransformComponent& transform)
{
	projectile.AddComponent<TransformComponent>(
		transform.x, transform.y,
		transform.lastDirection.x * projectileSpeed,
		transform.lastDirection.y * projectileSpeed
	);
	projectile.AddComponent<ProjectileComponent>(500.0f);
	projectile.AddComponent<DrawableComponent>(mFrames[0], sf::Vector2f(1, 1));
	projectile.AddComponent<AnimationComponent>(mFrames, 0.2f, true, -1.0f, true);
}

void MagicCharge::LoadTextures()
{
	mFrames = SpriteSheet::LoadTextures("assets/character/Charge.png", 64, 64);
	std::cout << mFrames.size() << std::endl;
}
