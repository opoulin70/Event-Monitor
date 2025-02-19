#include <iostream>
#include <vector>
#include <string>
#include "Device.h"  // Your Device class header
#include <systemd/sd-device.h>
#include <cstring>
#include "DeviceEnumerator.h"

// Helper function: Enumerate device syspaths (optionally filtered by subsystem)
std::vector<std::string> EnumerateDeviceSyspaths(const char* subsystemFilter = nullptr) {
    std::vector<std::string> paths;
    sd_device_enumerator* enumerator = nullptr;
    int r = sd_device_enumerator_new(&enumerator);

    if (r < 0) {
        std::cerr << "Failed to create sd_device_enumerator: " << strerror(-r) << std::endl;
        return paths;
    }
    
    // If a subsystem filter is provided (e.g., "usb"), add it.
    if (subsystemFilter) {
        r = sd_device_enumerator_add_match_subsystem(enumerator, subsystemFilter, 1);
        if (r < 0) {
            std::cerr << "Failed to add subsystem filter to sd_device_enumerator: " << strerror(-r) << std::endl;
            sd_device_enumerator_unref(enumerator);
            return paths;
        }
    }
    
    for (sd_device* dev = sd_device_enumerator_get_device_first(enumerator); dev != nullptr;
    dev = sd_device_enumerator_get_device_next(enumerator)) {
        const char* syspath = nullptr;
        r = sd_device_get_syspath(dev, &syspath);

        if (r >= 0 && syspath) {
            paths.push_back(syspath);
        } 
        else {
            std::cerr << "Failed to get syspath: " << strerror(-r) << std::endl;
        }
        
        //sd_device_unref(dev);
    }
    
    sd_device_enumerator_unref(enumerator);
    
    return paths;
}

void TestCreateDeviceFromStaticSyspath() {
    Device dev = Device::CreateFromSyspath("/sys/devices/pci0000:00/0000:00:0c.0/usb1/1-1");
    auto type = dev.GetType();
    auto name = dev.GetName();
    auto path = dev.GetPath();
    auto productID = dev.GetProductID();
    auto serial = dev.GetSerial();
    auto subsystem = dev.GetSubsystem();
    auto vendorID = dev.GetVendorID();

    std::cout << "Device Type: " << (type ? *type : "N/A") << "\n";
    std::cout << "Device Name: " << (name ? *name : "N/A") << "\n";
    std::cout << "Device Path: " << (path ? *path : "N/A") << "\n";
    std::cout << "Product ID: " << (productID ? *productID : "N/A") << "\n";
    std::cout << "Serial: " << (serial ? *serial : "N/A") << "\n";
    std::cout << "Subsystem: " << (subsystem ? *subsystem : "N/A") << "\n";
    std::cout << "Vendor ID: " << (vendorID ? *vendorID : "N/A") << "\n";
}

void TestCreateDevicesFromSyspaths() {
    // Optionally filter for a particular subsystem (e.g., "usb") or pass nullptr for all devices.
    auto syspaths = EnumerateDeviceSyspaths("usb");
    
    syspaths.empty() ? std::cerr << "No devices found in sd_device_enumerator.\n" : std::cout << "Found " << syspaths.size() << " devices:\n";
    
    // For each device, create a Device instance and test its getters.
    for (const auto& path : syspaths) {
        std::cout << "------------------------------\n";
        std::cout << "Device syspath: " << path << "\n";
        try {
            Device d = Device::CreateFromSyspath(path);

            const auto& typeOpt = d.GetType();
            if (typeOpt.has_value()) {
                std::cout << "Type: " << *typeOpt << "\n";
            } 
            else {
                std::cout << "Type: [Not available]\n";
            }

            const auto& serialOpt = d.GetSerial();
            if (serialOpt.has_value()) {
                std::cout << "Serial: " << *serialOpt << "\n";
            } 
            else {
                std::cout << "Serial: [Not available]\n";
            }

            const auto& nameOpt = d.GetName();
            if (nameOpt.has_value()) {
                std::cout << "Name: " << *nameOpt << "\n";
            } 
            else {
                std::cout << "Name: [Not available]\n";
            }
        } 
        catch (const std::exception& ex) {
            std::cerr << "Error initializing Device for " << path << ": " << ex.what() << "\n";
        }
    }
}

void TestEnumeratorAndDeviceRef() {
    std::vector<Device> devices;

    {
        DeviceEnumerator enumerator = DeviceEnumerator();
        enumerator.AddMatchSubsystem("usb", true);
        devices = enumerator.GetAllDevices();   
    }

    for (const auto& device : devices) {
        std::cout << "Device Name: " << device.GetName().value_or("N/A") << "\n";
        std::cout << "Device Path: " << device.GetPath().value_or("N/A") << "\n";
        std::cout << "Product ID: " << device.GetProductID().value_or("N/A") << "\n";
        std::cout << "Serial: " << device.GetSerial().value_or("N/A") << "\n";
        std::cout << "Subsystem: " << device.GetSubsystem().value_or("N/A") << "\n";
        std::cout << "Type: " << device.GetType().value_or("N/A") << "\n";
        std::cout << "Vendor ID: " << device.GetVendorID().value_or("N/A") << "\n";
    }
}

// void TestCreateDevicesFromSyspaths() {
//     Device dev;  // Assume this initializes your `sd_device`

//     std::cout << "Testing Device Getters...\n";

//     auto type = dev.GetType();
//     auto name = dev.GetName();
//     auto path = dev.GetPath();
//     auto productID = dev.GetProductID();
//     auto serial = dev.GetSerial();
//     auto subsystem = dev.GetSubsystem();
//     auto vendorID = dev.GetVendorID();

//     std::cout << "Device Type: " << (type ? *type : "N/A") << "\n";
//     std::cout << "Device Name: " << (name ? *name : "N/A") << "\n";
//     std::cout << "Device Path: " << (path ? *path : "N/A") << "\n";
//     std::cout << "Product ID: " << (productID ? *productID : "N/A") << "\n";
//     std::cout << "Serial: " << (serial ? *serial : "N/A") << "\n";
//     std::cout << "Subsystem: " << (subsystem ? *subsystem : "N/A") << "\n";
//     std::cout << "Vendor ID: " << (vendorID ? *vendorID : "N/A") << "\n";

//     std::cout << "Now testing cache behavior...\n";

//     // Call again to check if caching works
//     auto type2 = dev.GetType();
//     std::cout << "Re-fetched Type (should be cached): " << (type2 ? *type2 : "N/A") << "\n";

//     // Ensure it's returning the same object (cache check)
//     if (type == type2) {
//         std::cout << "Cache is working correctly!\n";
//     } else {
//         std::cout << "Cache is NOT working correctly!\n";
//     }

//     return;
// }

// void MonitorDevicesOld() {
//     struct udev *udev = udev_new();
//     if (!udev) {
//         std::cerr << "Failed to create udev object.\n";
//         return;
//     }

//     // Create a device enumerator
//     struct udev_enumerate *enumerate = udev_enumerate_new(udev);
//     udev_enumerate_add_match_subsystem(enumerate, "usb");  // Only list USB devices
//     udev_enumerate_scan_devices(enumerate);

//     struct udev_list_entry *devices, *dev_list_entry;
//     devices = udev_enumerate_get_list_entry(enumerate);

//     std::cout << "Connected USB devices:\n";

//     // Loop through the devices
//     udev_list_entry_foreach(dev_list_entry, devices) {
//         const char *path = udev_list_entry_get_name(dev_list_entry);
//         struct udev_device *dev = udev_device_new_from_syspath(udev, path);

//         if (dev) {
//             std::cout << "------------------------------------\n";
//             std::cout << "Device Path: " << path << "\n";
//             std::cout << "Subsystem: " << (udev_device_get_subsystem(dev) ? udev_device_get_subsystem(dev) : "Unknown") << "\n";
//             std::cout << "Devtype: " << (udev_device_get_devtype(dev) ? udev_device_get_devtype(dev) : "Unknown") << "\n";
//             std::cout << "Vendor ID: " << (udev_device_get_property_value(dev, "ID_VENDOR_ID") ? udev_device_get_property_value(dev, "ID_VENDOR_ID") : "Unknown") << "\n";
//             std::cout << "Product ID: " << (udev_device_get_property_value(dev, "ID_MODEL_ID") ? udev_device_get_property_value(dev, "ID_MODEL_ID") : "Unknown") << "\n";
//             std::cout << "Device Name: " << (udev_device_get_property_value(dev, "ID_MODEL") ? udev_device_get_property_value(dev, "ID_MODEL") : "Unknown") << "\n";
//             std::cout << "Device Serial: " << (udev_device_get_property_value(dev, "ID_SERIAL") ? udev_device_get_property_value(dev, "ID_SERIAL") : "Unknown") << "\n";

//             udev_device_unref(dev);
//         }
//     }

//     // Cleanup
//     udev_enumerate_unref(enumerate);
//     udev_unref(udev);

//     return;
// }
