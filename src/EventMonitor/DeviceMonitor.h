#pragma once
#include <memory>
#include <EventMonitor/Event.h>

extern "C" {
    #include <systemd/sd-device.h>
}

class DeviceMonitor {
public:
    explicit DeviceMonitor();
    explicit DeviceMonitor(const Event& eventLoop);
    ~DeviceMonitor() = default;
    DeviceMonitor(const DeviceMonitor&) = delete;
    DeviceMonitor(DeviceMonitor&&) noexcept = default;
    DeviceMonitor& operator=(const DeviceMonitor&) = delete;
    DeviceMonitor& operator=(DeviceMonitor&&) noexcept = default;

    // Attach the device monitor to the event loop.
    void AttachToEvent(const Event& eventLoop);
    // Detach the device monitor from the event loop.
    void DetachFromEvent();

    // Start monitoring for device events.
    void StartMonitoring();

private:
    void AttachToEventInternal();

    std::unique_ptr<sd_device_monitor, decltype(&sd_device_monitor_unref)> deviceMonitor;
    std::shared_ptr<Event> eventLoop;
};