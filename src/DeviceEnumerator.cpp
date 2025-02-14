#include "DeviceEnumerator.h"
#include <stdexcept>

DeviceEnumerator::DeviceEnumerator() 
    : enumerator(nullptr, &sd_device_enumerator_unref) {
    sd_device_enumerator* enumeratorTemp = nullptr;
    if (sd_device_enumerator_new(&enumeratorTemp) < 0) {
        throw std::runtime_error("Invalid DeviceEnumerator!");
    }
    enumerator.reset(enumeratorTemp);
}

std::optional<Device> DeviceEnumerator::GetDeviceFirst() {
    sd_device* dev = sd_device_enumerator_get_device_first(enumerator.get());
    if (!dev) {
        return std::nullopt;
    }
    sd_device_ref(dev); // Increment reference count to prevent deallocation when enumerator is destroyed.
    return Device(dev);
}

// TODO : Should return nullptr or something of the sort
std::optional<Device> DeviceEnumerator::GetDeviceNext() {
    sd_device* dev = sd_device_enumerator_get_device_next(enumerator.get());
    if (!dev) {
        return std::nullopt;
    }
    sd_device_ref(dev); // Increment reference count to prevent deallocation when enumerator is destroyed.
    return Device(dev);
}

std::vector<Device> DeviceEnumerator::GetAllDevices() {
    std::vector<Device> devices;
    for (sd_device* dev = sd_device_enumerator_get_device_first(enumerator.get()); 
    dev != nullptr;
    dev = sd_device_enumerator_get_device_next(enumerator.get())) {
        sd_device_ref(dev); // Increment reference count to prevent deallocation when enumerator is destroyed.
        devices.push_back(Device(dev));
    }
    return devices;
}

void DeviceEnumerator::AddMatchSubsystem(const std::string& subsystem, bool matchSubsystem) {
    if (sd_device_enumerator_add_match_subsystem(enumerator.get(), subsystem.c_str(), matchSubsystem) < 0) {
        throw std::runtime_error("Failed to add subsystem match!");
    }
}

void DeviceEnumerator::AddMatchSysattr(const std::string& sysattr, const std::string& value, bool matchSysattr) {
    if (sd_device_enumerator_add_match_sysattr(enumerator.get(), sysattr.c_str(), value.c_str(), matchSysattr) < 0) {
        throw std::runtime_error("Failed to add sysattr match!");
    }
}

void DeviceEnumerator::AddMatchProperty(const std::string& property, const std::string& value) {
    if (sd_device_enumerator_add_match_property(enumerator.get(), property.c_str(), value.c_str()) < 0) {
        throw std::runtime_error("Failed to add property match!");
    }
}

void DeviceEnumerator::AddMatchProperty_required(const std::string& property, const std::string& value) {
    if (sd_device_enumerator_add_match_property(enumerator.get(), property.c_str(), value.c_str()) < 0) {
        throw std::runtime_error("Failed to add required property match!");
    }
}

void DeviceEnumerator::AddMatchSysname(const std::string& sysname) {
    if (sd_device_enumerator_add_match_sysname(enumerator.get(), sysname.c_str()) < 0) {
        throw std::runtime_error("Failed to add sysname match!");
    }
}

void DeviceEnumerator::AddNomatchSysname(const std::string& sysname) {
    if (sd_device_enumerator_add_nomatch_sysname(enumerator.get(), sysname.c_str()) < 0) {
        throw std::runtime_error("Failed to add no-match sysname!");
    }
}

void DeviceEnumerator::AddMatchTag(const std::string& tag) {
    if (sd_device_enumerator_add_match_tag(enumerator.get(), tag.c_str()) < 0) {
        throw std::runtime_error("Failed to add tag match!");
    }
}