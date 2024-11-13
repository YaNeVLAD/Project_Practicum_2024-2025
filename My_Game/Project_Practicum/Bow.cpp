#include "Bow.h"
#include "Entity.h"
#include "Components.h"

void Bow::Attack(Entity& projectile, const TransformComponent & transform)
{
	projectile.AddComponent<DrawableComponent>(10, sf::Color::White);
	projectile.AddComponent<TransformComponent>(
		transform.x, transform.y, transform.vx * 1.25, transform.vy * 1.25);
	projectile.AddComponent<ProjectileComponent>();
}