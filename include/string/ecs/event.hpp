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

}  // namespace String