#pragma once

#include <string/core/component.hpp>
#include <string/core/entity.hpp>
#include <string/core/system.hpp>
#include <string/core/event.hpp>

#include <bits/stdc++.h>

namespace String {

class Coordinator {
public:
    void Init() {
        mComponentManager = std::make_unique<ComponentManager>();
        mEntityManager = std::make_unique<EntityManager>();
        mEventManager = std::make_unique<EventManager>();
        mSystemManager = std::make_unique<SystemManager>();
    }

    // Entity methods
    Entity CreateEntity() { return mEntityManager->create_entity(); }

    void DestroyEntity(Entity entity) {
        mEntityManager->destroy_entity(entity);

        mComponentManager->EntityDestroyed(entity);

        mSystemManager->EntityDestroyed(entity);
    }

    // Component methods
    template <typename T>
    void RegisterComponent() {
        mComponentManager->RegisterComponent<T>();
    }

    template <typename T>
    void AddComponent(Entity entity, T component) {
        mComponentManager->AddComponent<T>(entity, component);

        auto signature = mEntityManager->get_signature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), true);
        mEntityManager->set_signature(entity, signature);

        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    void RemoveComponent(Entity entity) {
        mComponentManager->RemoveComponent<T>(entity);

        auto signature = mEntityManager->get_signature(entity);
        signature.set(mComponentManager->GetComponentType<T>(), false);
        mEntityManager->set_signature(entity, signature);

        mSystemManager->EntitySignatureChanged(entity, signature);
    }

    template <typename T>
    T& GetComponent(Entity entity) {
        return mComponentManager->GetComponent<T>(entity);
    }

    template <typename T>
    ComponentType GetComponentType() {
        return mComponentManager->GetComponentType<T>();
    }

    // System methods
    template <typename T>
    std::shared_ptr<T> RegisterSystem() {
        return mSystemManager->RegisterSystem<T>();
    }

    template <typename T>
    void SetSystemSignature(Signature signature) {
        mSystemManager->SetSignature<T>(signature);
    }

    // Event methods
    void AddEventListener(EventId eventId, std::function<void(Event&)> const& listener) {
        mEventManager->AddListener(eventId, listener);
    }

    void SendEvent(Event& event) { mEventManager->SendEvent(event); }

    void SendEvent(EventId eventId) { mEventManager->SendEvent(eventId); }

private:
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<EventManager> mEventManager;
    std::unique_ptr<SystemManager> mSystemManager;
};

}  // namespace String