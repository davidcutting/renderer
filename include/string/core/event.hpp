#pragma once

#include <bits/stdc++.h>

namespace String {

using EventId = std::uint32_t;

class Event {
public:
    Event() = delete;

    explicit Event(EventId type) : type_(type) {}

    template <typename T>
    void set_param(EventId id, T value) {
        data_[id] = value;
    }

    template <typename T>
    T get_param(EventId id) {
        return std::any_cast<T>(data_[id]);
    }

    EventId get_type() const { return type_; }

private:
    EventId type_{};
    std::unordered_map<EventId, std::any> data_{};
};

class EventManager {
public:
    void add_listener(EventId event_id, std::function<void(Event&)> const& listener) {
        listeners_[event_id].push_back(listener);
    }

    void send_event(Event& event) {
        uint32_t type = event.get_type();

        for (auto const& listener : listeners_[type]) {
            listener(event);
        }
    }

    void send_event(EventId event_id) {
        Event event(event_id);

        for (auto const& listener : listeners_[event_id]) {
            listener(event);
        }
    }

private:
    std::unordered_map<EventId, std::list<std::function<void(Event&)>>> listeners_;
};

}  // namespace String