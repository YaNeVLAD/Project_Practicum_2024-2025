#pragma once

#include <stdexcept>
#include <string>

#include "Entity.h"

class EntityOutOfBoundsException : public std::runtime_error
{
public:
	EntityOutOfBoundsException(Entity entity)
		: runtime_error("Entity: " + std::to_string(entity) + " is out of bounds!") {}
};