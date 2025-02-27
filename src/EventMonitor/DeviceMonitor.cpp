#include <EventMonitor/DeviceMonitor.h>
#include <stdexcept>

// *** Public ***

DeviceMonitor::DeviceMonitor() 
    : deviceMonitor(nullptr, &sd_device_monitor_unref) {
    // Reference a new sd_device_monitor instance.
    sd_device_monitor* monitorTemp = nullptr;
    if (sd_device_monitor_new(&monitorTemp) < 0 || !monitorTemp) {
        throw std::runtime_error("Failed to create a DeviceEnumerator!");
    }
    deviceMonitor.reset(monitorTemp);
}

DeviceMonitor::DeviceMonitor(const Event& event)
    : DeviceMonitor() {
    // Attach the event loop to the device monitor.
    eventLoop = std::make_shared<Event>(event);
    AttachToEventInternal();
}

void DeviceMonitor::AttachToEvent(const Event& event) {
    eventLoop = std::make_shared<Event>(event);
    AttachToEventInternal();
}

void DeviceMonitor::DetachFromEvent() {
    if (sd_device_monitor_detach_event(deviceMonitor.get()) < 0) {
        throw std::runtime_error("Failed to detach DeviceMonitor from event loop : sd_device_monitor_detach_event failed!");
    }
}

void DeviceMonitor::StartMonitoring() {
    // Check if the event loop is set.
    if (!eventLoop) {
        throw std::runtime_error("Failed to start monitoring : Event ptr is null!");
    }

    // Start monitoring for device events.
    if (sd_device_monitor_start(deviceMonitor.get(), nullptr, nullptr) < 0) {
        throw std::runtime_error("Failed to start monitoring : sd_device_monitor_start failed!");
    }
}

// *** Private ***

void DeviceMonitor::AttachToEventInternal() {
    // Check if the event loop is set.
    if (!eventLoop) {
        throw std::runtime_error("Failed to attach DeviceMonitor to event loop : Event ptr is null!");
    }

    // Attach the device monitor to the event loop.
    if (sd_device_monitor_attach_event(deviceMonitor.get(), eventLoop.get()->GetEvent()) < 0) {
        throw std::runtime_error("Failed to attach DeviceMonitor to event loop : sd_device_monitor_attach_event failed!");
    }
}