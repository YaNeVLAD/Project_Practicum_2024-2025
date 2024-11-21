#include "WeaponFactory.h"
#include "../../Entity/Weapon/Book/Book.h"
#include "../../Entity/Weapon/MagicCharge/MagicCharge.h"

std::unique_ptr<Weapon> WeaponFactory::Create(const std::string& weaponName)
{
	if (weaponName == "Holy Book")
	{
		return std::make_unique<Book>();
	}
	if (weaponName == "Magic Charge")
	{
		return std::make_unique<MagicCharge>();
	}

	throw std::invalid_argument("Unknown weapon type " + weaponName);
}