#pragma once
#include <memory>
#include <functional>
#include <EventMonitor/Event.h>
#include <EventMonitor/Device.h>

extern "C" {
    #include <systemd/sd-device.h>
}

using DeviceEventCallback = std::function<void(Device device)>;

class DeviceMonitor {
public:
    explicit DeviceMonitor();
    explicit DeviceMonitor(std::shared_ptr<Event> eventLoop);
    ~DeviceMonitor() = default;
    DeviceMonitor(const DeviceMonitor&) = delete;
    DeviceMonitor(DeviceMonitor&&) noexcept = default;
    DeviceMonitor& operator=(const DeviceMonitor&) = delete;
    DeviceMonitor& operator=(DeviceMonitor&&) noexcept = default;

    void SetCallback(const DeviceEventCallback callback);

    // Attach the device monitor to the event loop.
    void AttachToEvent(std::shared_ptr<Event> eventLoop);
    // Detach the device monitor from the event loop.
    void DetachFromEvent();
    // TODO : GetEvent()

    // Start monitoring for device events.
    void StartMonitoring();

private:
    std::unique_ptr<sd_device_monitor, decltype(&sd_device_monitor_unref)> deviceMonitor;
    std::shared_ptr<Event> eventLoop;

    DeviceEventCallback userCallback;

#ifdef ENABLE_TESTS
    friend class DeviceMonitorTest;
#endif // ENABLE_TESTS
};