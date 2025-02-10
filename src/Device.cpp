#include "Device.h"
#include <stdexcept>
#include <functional>
#include <cassert>

// *** Public ***

// Constructor: Initializes the device object
Device::Device(const std::string& path) 
    : device(nullptr, &sd_device_unref) 
{
    sd_device* dev = nullptr;
    if (sd_device_new_from_syspath(&dev, path.c_str()) < 0) {
        throw std::runtime_error("Failed to get device from sd-device");
    }
    // device = std::make_unique<sd_device*>(dev);
    device.reset(dev);
}

const std::optional<std::string>& Device::GetName(bool refreshCache) const {
    return GetCachedValueOrFetch(name, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_sysname(device.get(), &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetPath(bool refreshCache) const {
    return GetCachedValueOrFetch(path, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_devpath(device.get(), &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetProductID(bool refreshCache) const {
    return GetCachedValueOrFetch(productID, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_property_value(device.get(), "ID_MODEL_ID", &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetSerial(bool refreshCache) const {
    return GetCachedValueOrFetch(serial, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_property_value(device.get(), "ID_SERIAL", &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetSubsystem(bool refreshCache) const {
    return GetCachedValueOrFetch(subsystem, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_subsystem(device.get(), &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetType(bool refreshCache) const {
    return GetCachedValueOrFetch(type, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_devtype(device.get(), &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetVendorID(bool refreshCache) const {
    return GetCachedValueOrFetch(
    vendorID, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_property_value(device.get(), "ID_VENDOR_ID", &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

void Device::InvalidateCache() {
    name.reset();
    path.reset();
    productID.reset();
    serial.reset();
    subsystem.reset();
    type.reset();
    vendorID.reset();
}

// *** Private ***

template <typename T, typename GetterFunc>
auto Device::GetCachedValueOrFetch(std::optional<T>& cache, GetterFunc&& getter, bool refreshCache) const
-> std::conditional_t<(sizeof(T) > sizeof(void*)), const std::optional<T>&, std::optional<T>> {
    if ((!cache.has_value() || refreshCache) && device) {
        cache = std::invoke(std::forward<GetterFunc>(getter));
    }

    return cache;
}