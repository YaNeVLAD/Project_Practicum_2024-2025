#include "SFML/Graphics.hpp"
#include "Components.h"
#include "Weapon.h"

void WeaponComponent::AddWeapon(std::unique_ptr<Weapon> weapon)
{
	weapons.push_back(std::move(weapon));
}