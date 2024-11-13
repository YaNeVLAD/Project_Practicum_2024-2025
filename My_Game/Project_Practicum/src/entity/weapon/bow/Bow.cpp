#include "Bow.h"
#include "../../../entity/Entity.h"
#include "../../../component/Components.h"

void Bow::Attack(Entity& projectile, const TransformComponent& transform)
{
	projectile.AddComponent<DrawableComponent>(10, 10, sf::Color::White);
	projectile.AddComponent<TransformComponent>(
		transform.x, transform.y, transform.vx * 1.25f, transform.vy * 1.25f);
	projectile.AddComponent<ProjectileComponent>(500.0f);
}

void Bow::LoadTextures()
{
	
}
