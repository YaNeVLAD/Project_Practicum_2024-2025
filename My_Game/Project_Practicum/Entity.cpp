#include "Entity.h"

//template<typename ComponentType, typename... Args>
//void Entity::AddComponent(Args&&... args)
//{
//	mComponents.emplace(std::type_index(typeid(ComponentType)), std::make_unique<ComponentType>(std::forward<Args>(args)...));
//}
//
//template<typename ComponentType>
//ComponentType* Entity::GetComponent()
//{
//	auto it = mComponents.find(std::type_index(typeid(ComponentType)));
//	return it != mComponents.end() ? static_cast<ComponentType*>(it->second.get()) : nullptr;
//}
//
//Entity::Entity(IdType id) : mId(id) {}