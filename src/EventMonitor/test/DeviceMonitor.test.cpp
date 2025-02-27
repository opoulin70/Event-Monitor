#include <gtest/gtest.h>
#include <EventMonitor/DeviceMonitor.h>

TEST(DeviceMonitorTests, MultipleDeviceMonitorToOneEvent) {
    Event event = Event();

    DeviceMonitor monitor1 = DeviceMonitor(event);
    DeviceMonitor monitor2 = DeviceMonitor(event);
    
    // TODO
}

// TODO : Should not be able to start event loop without having attached it to a device monitor.