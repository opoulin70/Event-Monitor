#pragma once

#include "Device.h"
#include <optional>
#include <string>
#include <memory>
#include <vector>

extern "C" {
    #include <systemd/sd-device.h>
}

class DeviceEnumerator {
public:
    explicit DeviceEnumerator();
    ~DeviceEnumerator() = default;
    DeviceEnumerator(const DeviceEnumerator&) = delete;
    DeviceEnumerator(DeviceEnumerator&&) noexcept = default;
    DeviceEnumerator& operator=(const DeviceEnumerator&) = delete;
    DeviceEnumerator& operator=(DeviceEnumerator&&) noexcept = default;

    std::optional<Device> GetDeviceFirst() const;
    std::optional<Device> GetDeviceNext()const;
    std::vector<Device> GetAllDevices() const;
    std::optional<Device> GetSubsystemFirst(); // TODO : Implement
    std::optional<Device> GetSubsystemNext(); // TODO : Implement
    std::vector<Device> GetAllSubsystems(); // TODO : Implement

    // TODO : Should probably remove assert and replace by log error.
    void AddMatchSubsystem(const std::string& subsystem, bool matchSubsystem);
    void AddMatchSysattr(const std::string& sysattr, const std::string& value, bool matchSysattr);
    void AddMatchProperty_required(const std::string& property, const std::string& value);
    void AddMatchProperty(const std::string& property, const std::string& value);
    void AddMatchSysname(const std::string& sysname);
    void AddNomatchSysname(const std::string& sysname);
    void AddMatchTag(const std::string& tag);
    void AddMatchParent(sd_device_enumerator *enumerator, sd_device *parent); // TODO : Implement

private:
    std::unique_ptr<sd_device_enumerator, decltype(&sd_device_enumerator_unref)> enumerator;
};