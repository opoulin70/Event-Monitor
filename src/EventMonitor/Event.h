#pragma once

#include <memory>

extern "C" {
    #include <systemd/sd-event.h>
}

// TODO : Without adding new functionalities, this class could be replaced by directly using std::shared_ptr<sd_event>
// TODO : However, it does provide the custom destructor implementation and could be used to add new functionalities in the future.
class Event {
public:
    explicit Event();
    ~Event() = default;
    Event(const Event&) = delete;
    Event(Event&&) noexcept = default;
    Event& operator=(const Event&) = delete;
    Event& operator=(Event&&) noexcept = default;

    sd_event* GetEvent() const {return eventLoop.get();}

private:
    std::unique_ptr<sd_event, decltype(&sd_event_unref)> eventLoop;
};