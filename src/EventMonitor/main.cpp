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

int main() {
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

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}