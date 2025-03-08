#pragma once

void TestCreateDeviceFromStaticSyspath(const std::string& syspath = "/sys/devices/pci0000:00/0000:00:0c.0/usb1/1-1");
void TestCreateDevicesFromSyspaths();
void TestEnumeratorAndDeviceRef();
int TestListenToDevicesPlugUnplug();
int TestListenToDevicesPlugUnplug_sdDevice();