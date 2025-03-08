#include <systemd/sd-device.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <EventMonitor/DeviceEnumerator.h>
#include <EventMonitor/Device.h>
#include <EventMonitor/DeviceMonitor.h>

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
        
        //sd_device_unref(dev); // This causes crash.
    }
    
    sd_device_enumerator_unref(enumerator);
    
    return paths;
}

std::string DeviceActionToString(sd_device_action_t action) {
    switch (action) {
        case SD_DEVICE_ADD: return "Add";
        case SD_DEVICE_REMOVE: return "Remove";
        case SD_DEVICE_CHANGE: return "Change";
        case SD_DEVICE_MOVE: return "Move";
        case SD_DEVICE_ONLINE: return "Online";
        case SD_DEVICE_OFFLINE: return "Offline";
        case SD_DEVICE_BIND: return "Bind";
        case SD_DEVICE_UNBIND: return "Unbind";
        default: return "Unknown";
    }
}

void TestCreateDeviceFromStaticSyspath(const std::string& syspath = "/sys/devices/pci0000:00/0000:00:0c.0/usb1/1-1") {
    Device dev = Device::CreateFromSyspath(syspath);
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

int TestListenToDevicesPlugUnplug() {
    try {
        // Create the event loop
        auto eventLoop = std::make_shared<Event>();

        // Create the device monitor and attach it to the event loop
        DeviceMonitor monitor(eventLoop);

        // Set a callback to handle USB device events
        monitor.SetCallback([](const DeviceMonitor& monitorRef, Device device) {
            (void) monitorRef; // Unused.
            const auto& action = device.GetAction(); // Assuming `GetAction()` returns "add", "remove", etc.
            const auto& name = device.GetName();     // Assuming `GetName()` provides the device name

            std::cout << "Callback called!" << std::endl;

            if (!action.has_value()) {
                return;
            }
            
            if (action.value() == sd_device_action_t::SD_DEVICE_ADD) {
                std::cout << "USB Device Connected: " << name.value_or("ERROR : Can't find name.") << std::endl;
            } else if (action.value() == sd_device_action_t::SD_DEVICE_REMOVE) {
                std::cout << "USB Device Disconnected: " << name.value_or("ERROR : Can't find name.") << std::endl;
            } else {
                std::cout << "Unknown event (" << DeviceActionToString(action.value()) << ") for device: " << name.value_or("ERROR : Can't find name.") << std::endl;
            }
        });

        // Start monitoring for USB events
        monitor.StartMonitoring();
        std::cout << "Monitoring USB devices... Press Ctrl+C to stop." << std::endl;

        // Run the event loop indefinitely
        sd_event_loop(eventLoop->GetEvent());

    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int TestListenToDevicesPlugUnplug_sdDevice() {
    try {
        // Create a simple device monitor
        sd_device_monitor* monitor = nullptr;
        if (sd_device_monitor_new(&monitor) < 0 || !monitor) {
            throw std::runtime_error("Failed to create a DeviceMonitor instance!");
        }

        // Attach the monitor to the event loop
        sd_event* eventLoop = nullptr;
        if (sd_event_default(&eventLoop) < 0 || !eventLoop) {
            throw std::runtime_error("Failed to create an event loop!");
        }

        sd_device_monitor_attach_event(monitor, eventLoop);

        // Start monitoring for device events
        int result = sd_device_monitor_start(
            monitor, 
            [](sd_device_monitor* monitor, sd_device* device, void* userdata) -> int {
                (void) monitor; // Unused.
                (void) userdata; // Unused.
                
                std::cout << "Callback called!" << std::endl;

                if (!device) {
                    std::cout << "Invalid device!" << std::endl;
                    return -1;
                }

                sd_device_action_t action;
                if (sd_device_get_action(device, &action) < 0) {
                    std::cerr << "Failed to get device action!" << std::endl;
                    return -1;
                }
                
                if (action) {
                    if (action == sd_device_action_t::SD_DEVICE_ADD) {
                        std::cout << "Device added!" << std::endl;
                    } else if (action == sd_device_action_t::SD_DEVICE_REMOVE) {
                        std::cout << "Device removed!" << std::endl;
                    }
                }

                return 0;
            },
            nullptr
        );

        if (result < 0) {
            throw std::runtime_error("Failed to start monitoring: sd_device_monitor_start failed!");
        }

        std::cout << "Monitoring devices..." << std::endl;

        sd_event_loop(eventLoop);

        // Clean up
        sd_device_monitor_unref(monitor);
        sd_event_unref(eventLoop);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
