#include <EventMonitor/Event.h>
#include <stdexcept>

Event::Event() 
: eventLoop(nullptr, &sd_event_unref) {
    sd_event* eventLoopTemp = nullptr;
    if (sd_event_default(&eventLoopTemp) < 0 || !eventLoopTemp) {
        throw std::runtime_error("Failed to create an Event!");
    }
    eventLoop.reset(eventLoopTemp);
}