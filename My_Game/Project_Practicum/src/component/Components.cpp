#include "Components.h"

#include "../entity/weapon/Weapon.h"
#include "../Entity/Entity.h"

void WeaponComponent::AddWeapon(std::unique_ptr<Weapon> weapon)
{
	weapons.push_back(std::move(weapon));
}