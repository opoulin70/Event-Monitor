#include "Device.h"
#include <stdexcept>
#include <functional>
#include <cassert>

// *** Public ***

Device Device::CreateFromSyspath(const std::string& syspath) {
    sd_device* dev = nullptr;
    if (sd_device_new_from_syspath(&dev, syspath.c_str()) < 0) {
        throw std::runtime_error("Failed to create device from syspath!");
    }
    return Device(dev);
}

Device Device::CreateFromDevnum(char type, dev_t devnum) {
    sd_device* dev = nullptr;
    if (sd_device_new_from_devnum(&dev, type, devnum) < 0) {
        throw std::runtime_error("Failed to create device from devnum!");
    }
    return Device(dev);
}

Device Device::CreateFromSubsystemSysname(const std::string& subsystem, const std::string& sysname) {
    sd_device* dev = nullptr;
    if (sd_device_new_from_subsystem_sysname(&dev, subsystem.c_str(), sysname.c_str()) < 0) {
        throw std::runtime_error("Failed to create device from subsystem and sysname!");
    }
    return Device(dev);
}

Device Device::CreateFromDeviceId(const std::string& id) {
    sd_device* dev = nullptr;
    if (sd_device_new_from_device_id(&dev, id.c_str()) < 0) {
        throw std::runtime_error("Failed to create device from device ID!");
    }
    return Device(dev);
}

Device Device::CreateFromStatRdev(const struct stat& st) {
    sd_device* dev = nullptr;
    if (sd_device_new_from_stat_rdev(&dev, &st) < 0) {
        throw std::runtime_error("Failed to create device from stat rdev!");
    }
    return Device(dev);
}

Device Device::CreateFromDevname(const std::string& devname) {
    sd_device* dev = nullptr;
    if (sd_device_new_from_devname(&dev, devname.c_str()) < 0) {
        throw std::runtime_error("Failed to create device from devname!");
    }
    return Device(dev);
}

Device Device::CreateFromPath(const std::string& path) {
    sd_device* dev = nullptr;
    if (sd_device_new_from_path(&dev, path.c_str()) < 0) {
        throw std::runtime_error("Failed to create device from path!");
    }
    return Device(dev);
}

Device Device::CreateFromIfname(const std::string& ifname) {
    sd_device* dev = nullptr;
    if (sd_device_new_from_ifname(&dev, ifname.c_str()) < 0) {
        throw std::runtime_error("Failed to create device from ifname!");
    }
    return Device(dev);
}

Device Device::CreateFromIfindex(int ifindex) {
    sd_device* dev = nullptr;
    if (sd_device_new_from_ifindex(&dev, ifindex) < 0) {
        throw std::runtime_error("Failed to create device from ifindex!");
    }
    return Device(dev);
}

const std::optional<std::string>& Device::GetName(const bool refreshCache) const {
    return GetCachedValueOrFetch(name, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_sysname(device.get(), &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetPath(const bool refreshCache) const {
    return GetCachedValueOrFetch(path, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_devpath(device.get(), &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetProductID(const bool refreshCache) const {
    return GetCachedValueOrFetch(productID, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_property_value(device.get(), "ID_MODEL_ID", &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetSerial(const bool refreshCache) const {
    return GetCachedValueOrFetch(serial, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_property_value(device.get(), "ID_SERIAL", &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetSubsystem(const bool refreshCache) const {
    return GetCachedValueOrFetch(subsystem, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_subsystem(device.get(), &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetType(const bool refreshCache) const {
    return GetCachedValueOrFetch(type, 
    [this]() { 
        const char* val = nullptr;
        return (sd_device_get_devtype(device.get(), &val) >= 0) ? std::make_optional(val) : std::nullopt;
    },
    refreshCache);
}

const std::optional<std::string>& Device::GetVendorID(const bool refreshCache) const {
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

Device::Device(sd_device* dev)
    : device(dev, &Device::DeviceUnref) {
    if (!dev) {
        throw std::runtime_error("Invalid Device!");
    }
}

void Device::DeviceUnref(sd_device* dev) {
    dev ? sd_device_unref(dev) : throw std::runtime_error("Tried to unreference a Device already unreferenced!");
}

template <typename T, typename GetterFunc>
auto Device::GetCachedValueOrFetch(std::optional<T>& cache, GetterFunc&& getter, bool refreshCache) const
-> std::conditional_t<(sizeof(T) > sizeof(void*)), const std::optional<T>&, std::optional<T>> {
    if ((!cache.has_value() || refreshCache) && device) {
        cache = std::invoke(std::forward<GetterFunc>(getter));
    }

    return cache;
}