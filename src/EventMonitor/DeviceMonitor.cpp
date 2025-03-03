#include <stdexcept>
#include <iostream>
#include <EventMonitor/DeviceMonitor.h>

// *** Public ***

DeviceMonitor::DeviceMonitor() 
    : isMonitoring(false),
      deviceMonitor(nullptr, &sd_device_monitor_unref),
      eventLoop(nullptr),
      userCallback(nullptr) {
    // Reference a new sd_device_monitor instance.
    sd_device_monitor* monitorTemp = nullptr;
    if (sd_device_monitor_new(&monitorTemp) < 0 || !monitorTemp) {
        throw std::runtime_error("Failed to create a DeviceEnumerator!");
    }
    deviceMonitor.reset(monitorTemp);
}

DeviceMonitor::DeviceMonitor(std::shared_ptr<Event> event)
    : DeviceMonitor() { 
    std::cout << "Building DeviceMonitor..." << std::endl; // TODO : REMOVE
    AttachToEvent(std::move(event));
}

void DeviceMonitor::SetCallback(const DeviceEventCallback callback) {
    if (!callback) {
        throw std::invalid_argument("Failed to set callback : Callback cannot be null!");
    }
    userCallback = std::move(callback);
    std::cout << "Callback set!" << std::endl; // TODO : REMOVE
}

void DeviceMonitor::AttachToEvent(std::shared_ptr<Event> event) {
    // Check if the event is valid.
    if (!event) {
        throw std::runtime_error("Failed to attach DeviceMonitor to event loop : Event ptr is null!");
    }
    if (!event->GetEvent()) {
        throw std::runtime_error("Failed to attach DeviceMonitor to event loop : sd_event* in Event is null!");
    }

    eventLoop = event;

    // Attach the device monitor to the event loop.
    if (sd_device_monitor_attach_event(deviceMonitor.get(), eventLoop.get()->GetEvent()) < 0) {
        throw std::runtime_error("Failed to attach DeviceMonitor to event loop : sd_device_monitor_attach_event failed!");
    }
    std::cout << "Attached to event!" << std::endl; // TODO : REMOVE
}

void DeviceMonitor::DetachFromEvent() {
    // TODO : Write warning in else when calling this on !eventLoop ?
    if (eventLoop) {
        if (sd_device_monitor_detach_event(deviceMonitor.get()) < 0) {
            throw std::runtime_error("Failed to detach DeviceMonitor from event loop : sd_device_monitor_detach_event failed!");
        }
        eventLoop.reset();
    }
}

void DeviceMonitor::StartMonitoring() {
    std::cout << "Trying to start monitoring..." << std::endl; // TODO : REMOVE
    if (isMonitoring) {
        // TODO : log warning (already monitoring...) and return?
        throw std::runtime_error("Failed to start monitoring : Already monitoring!");
    }

    // Check if the event loop and callback are set.
    if (!eventLoop) {
        throw std::runtime_error("Failed to start monitoring : Event ptr is null!");
    }
    if (!userCallback) {
        throw std::runtime_error("Failed to start monitoring : Callback is not set!");
    }

    // Set the callback.
    const int result = sd_device_monitor_start(
        deviceMonitor.get(), 
        [](sd_device_monitor* monitor, sd_device* device, void* userdata) -> int {
            (void) monitor; // Unused.
            std::cout << "Callback being called..." << std::endl; // TODO : REMOVE

            auto* self = static_cast<DeviceMonitor*>(userdata);
            if (!self) {
                return -1;
            }

            self->userCallback(*self, Device(device));
            std::cout << "Callback has been triggered.." << std::endl; // TODO : REMOVE

            return 0;
        },
        this
    ); 
    
    if (result < 0){
        throw std::runtime_error("Failed to start monitoring : sd_device_monitor_start failed!");
    }

    isMonitoring = true;

    std::cout << "Monitoring started!" << std::endl; // TODO : REMOVE
}

void DeviceMonitor::StopMonitoring() {
    if (!isMonitoring) {
        // TODO : log warning ?
        return;
    }

    // Stop monitoring for device events.
    if (sd_device_monitor_stop(deviceMonitor.get()) < 0) {
        throw std::runtime_error("Failed to stop monitoring : sd_device_monitor_stop failed!");
    }

    isMonitoring = false;
}