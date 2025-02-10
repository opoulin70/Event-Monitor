#pragma once

#include <string>
#include <memory>
#include <optional>

extern "C" {
    #include <systemd/sd-device.h>
}

class Device {
public:
    explicit Device(const std::string& path);
    ~Device() = default;
    Device(const Device&) = delete;
    Device(Device&&) noexcept = default;
    Device& operator=(const Device&) = delete;
    Device& operator=(Device&&) noexcept = default;

    const std::optional<std::string>& GetName(bool refreshCache = false) const;
    const std::optional<std::string>& GetPath(bool refreshCache = false) const;
    const std::optional<std::string>& GetProductID(bool refreshCache = false) const;
    const std::optional<std::string>& GetSerial(bool refreshCache = false) const;
    const std::optional<std::string>& GetSubsystem(bool refreshCache = false) const;
    const std::optional<std::string>& GetType(bool refreshCache = false) const;
    const std::optional<std::string>& GetVendorID(bool refreshCache = false) const;

    // TODO: Use boolean to indicate if cache is stale ?
    void InvalidateCache();

private:
    // Generic caching helper function.
    //
    // Returns cached value if present, otherwise invoke the provided getter function to fetch and cache the value.
    // If the device is unplugged (device == nullptr), the function returns the last cached value.
    // Returns const reference for large types (e.g., std::string), otherwise returns by value.
    // 
    // @param refreshCache: If true, the cache is invalidated before fetching the value.
    /**
     * Retrieves a cached value or fetches it if necessary.
     *
     * @tparam T The type of the cached value.
     * @tparam GetterFunc A callable function that returns std::optional<T>.
     * @param cache The cached value to check or update.
     * @param getter The function used to fetch the value if caching conditions are met.
     * @param refreshCache If true, forces a re-fetch of the value even if the cache is populated.
     * @return Either a reference to the cached value (if T is large e.g. std::string) or a new copy (if T is small).
     */
    template <typename T, typename GetterFunc>
    auto GetCachedValueOrFetch(std::optional<T>& cache, GetterFunc&& getter, bool refreshCache = false) const
    -> std::conditional_t<(sizeof(T) > sizeof(void*)), const std::optional<T>&, std::optional<T>>;


    std::unique_ptr<sd_device, decltype(&sd_device_unref)> device;

    mutable std::optional<std::string> name;
    mutable std::optional<std::string> path;
    mutable std::optional<std::string> productID;
    mutable std::optional<std::string> serial;
    mutable std::optional<std::string> subsystem;
    mutable std::optional<std::string> type;
    mutable std::optional<std::string> vendorID;
};