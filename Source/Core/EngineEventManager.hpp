#pragma once

#include <vector>
#include <memory>
#include <mutex>

namespace TulparEngine {
    class Event {
    public:
        virtual ~Event() {}
    };

    class EventManager {
    public:
        static EventManager& GetInstance() {
            static EventManager instance;
            return instance;
        }

        void AddEvent(std::shared_ptr<Event> event) {
            std::lock_guard<std::mutex> lock(mutex_);
            events_.push_back(event);
        }

        const std::vector<std::shared_ptr<Event>>& GetEvents() const {
            return events_;
        }

        void ClearEvents() {
            std::lock_guard<std::mutex> lock(mutex_);
            events_.clear();
        }

    private:
        EventManager() {}
        ~EventManager() {}
        std::vector<std::shared_ptr<Event>> events_;
        std::mutex mutex_;
    };
}