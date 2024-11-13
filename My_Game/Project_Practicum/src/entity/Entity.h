#pragma once

#include <cstddef>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "../component/Components.h"

class Entity {
public:
    using IdType = std::size_t;

    explicit Entity(IdType id) : mId(id) {}

    // Запрещаем копирование
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    // Разрешаем перемещение
    Entity(Entity&&) noexcept = default;
    Entity& operator=(Entity&&) noexcept = default;

    template<typename ComponentType, typename... Args>
    void AddComponent(Args&&... args) 
    {
        mComponents.emplace(std::type_index(typeid(ComponentType)), std::make_unique<ComponentType>(std::forward<Args>(args)...));
    }

    template<typename ComponentType>
    ComponentType* GetComponent() 
    {
        auto it = mComponents.find(std::type_index(typeid(ComponentType)));
        return it != mComponents.end() ? static_cast<ComponentType*>(it->second.get()) : nullptr;
    }

    IdType GetId() const { return mId; }

private:
    IdType mId;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> mComponents;
};