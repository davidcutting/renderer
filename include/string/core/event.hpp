#pragma once

#include <bits/stdc++.h>

namespace String {

using EventId = std::uint32_t;

class Event {
public:
    Event() = delete;

    explicit Event(EventId type) : mType(type) {}

    template <typename T>
    void SetParam(EventId id, T value) {
        mData[id] = value;
    }

    template <typename T>
    T GetParam(EventId id) {
        return std::any_cast<T>(mData[id]);
    }

    EventId GetType() const { return mType; }

private:
    EventId mType{};
    std::unordered_map<EventId, std::any> mData{};
};

class EventManager {
public:
    void AddListener(EventId eventId, std::function<void(Event&)> const& listener) {
        listeners[eventId].push_back(listener);
    }

    void SendEvent(Event& event) {
        uint32_t type = event.GetType();

        for (auto const& listener : listeners[type]) {
            listener(event);
        }
    }

    void SendEvent(EventId eventId) {
        Event event(eventId);

        for (auto const& listener : listeners[eventId]) {
            listener(event);
        }
    }

private:
    std::unordered_map<EventId, std::list<std::function<void(Event&)>>> listeners;
};

}  // namespace String