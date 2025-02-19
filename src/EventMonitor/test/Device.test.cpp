#include <EventMonitor/Device.h>
#include "gtest/gtest.h"

TEST(DeviceTests, CreateDevice) {
    Device dev = Device::CreateFromSyspath("/sys/devices/pci0000:00/0000:00:0c.0/usb1/1-1");
    EXPECT_EQ(dev.GetSyspath(), "/sys/devices/pci0000:00/0000:00:0c.0/usb1/1-1");
}