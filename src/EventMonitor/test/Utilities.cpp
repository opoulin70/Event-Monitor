

/*
#include <EventMonitor/test/Utilities.h>
#include <stdexcept>
#include <filesystem>

// *** FakeDeviceManager ***

FakeDeviceManager::FakeDeviceManager() {
    char temp[] = "/tmp/fake-sysfs-XXXXXX";

    char* res = mkdtemp(temp);
    res ? sysfsDirPath = res : throw std::runtime_error("Failed to create temporary sysfs directory for testing.");

    // devicePath = sysfsDirPath + "/class/input/" + deviceName;
    // fs::create_directories(devicePath);
}

FakeDeviceManager::~FakeDeviceManager() {
    std::filesystem::remove_all(sysfsDirPath);
}

std::string FakeDeviceManager::CreateFakeDevice(const std::string& type, const std::string& name) {
    std::string devicePath = sysfsDirPath + "/class/" + type + "/" + name;

    std::filesystem::create_directories(devicePath);
    devices.push_back(devicePath);

    return devicePath
}
*/