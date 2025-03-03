#include <iostream>
#include <EventMonitor/TestMonitor.h>
#include <EventMonitor/DeviceMonitor.h>

// TODO : Remove.
void DisplayTestName (const std::string& testName) {
    std::cout << "*********************************" << std::endl;
    std::cout << "Testing " << testName << "..." << std::endl;
    std::cout << "*********************************" << std::endl;
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

int Test1() {
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
        // while (true) {
        //     int ret = sd_event_wait(eventLoop->GetEvent(), 0);
        //     if (ret < 0) {
        //         std::cerr << "Error waiting for event: " << strerror(-ret) << std::endl;
        //         break;  // You can decide how you want to handle this error.
        //     } else {
        //         std::cout << "Event triggered!" << std::endl;
        //     }
        // }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Test2() {
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

                sd_device_action_t* action = nullptr;
                if (sd_device_get_action(device, action) < 0) {
                    std::cerr << "Failed to get device action!" << std::endl;
                    return -1;
                }
                
                if (action) {
                    if (*action == sd_device_action_t::SD_DEVICE_ADD) {
                        std::cout << "Device added!" << std::endl;
                    } else if (*action == sd_device_action_t::SD_DEVICE_REMOVE) {
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
        // Wait for events
        // while (true) {
        //     int ret = sd_event_wait(eventLoop, 0);
        //     if (ret < 0) {
        //         std::cerr << "Error waiting for event: " << strerror(-ret) << std::endl;
        //         break;
        //     } else {
        //         std::cout << "Event triggered!" << std::endl;
        //     }
        // }

        // Clean up
        sd_device_monitor_unref(monitor);
        sd_event_unref(eventLoop);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main() {
    return Test1();
    //return Test2();
}