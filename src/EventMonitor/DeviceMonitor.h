#pragma once
#include <memory>
#include <functional>
#include <EventMonitor/Event.h>
#include <EventMonitor/Device.h>

extern "C" {
    #include <systemd/sd-device.h>
}


class DeviceMonitor {
public:
    using DeviceEventCallback = std::function<void(const DeviceMonitor&, Device)>;

    explicit DeviceMonitor();
    explicit DeviceMonitor(std::shared_ptr<Event> eventLoop);
    ~DeviceMonitor() = default;
    DeviceMonitor(const DeviceMonitor&) = delete;
    DeviceMonitor(DeviceMonitor&&) noexcept = default;
    DeviceMonitor& operator=(const DeviceMonitor&) = delete;
    DeviceMonitor& operator=(DeviceMonitor&&) noexcept = default;

    const std::shared_ptr<Event>& GetEvent() const { return eventLoop; }
    
    // Copy the callback used during the event loop.
    void SetCallback(const DeviceEventCallback callback);
    
    bool IsAttachedToEvent() const { return eventLoop != nullptr; }
    bool IsMonitoringForEvents() const { return isMonitoring; }

    // Attach the device monitor to the event loop.
    void AttachToEvent(std::shared_ptr<Event> eventLoop);
    // Detach the device monitor from the event loop.
    void DetachFromEvent();

    // Start monitoring for device events.
    void StartMonitoring();
    // Stop monitoring for device events.
    void StopMonitoring();

private:
    bool isMonitoring;

    std::unique_ptr<sd_device_monitor, decltype(&sd_device_monitor_unref)> deviceMonitor;
    std::shared_ptr<Event> eventLoop;

    DeviceEventCallback userCallback;

#ifdef ENABLE_TESTS
    friend class DeviceMonitorTest;
#endif // ENABLE_TESTS
};