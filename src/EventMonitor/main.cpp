#include <iostream>
#include <EventMonitor/TestMonitor.h>

// TODO : Remove.
void DisplayTestName (const std::string& testName) {
    std::cout << "Testing " << testName << "..." << std::endl;
    std::cout << "*********************************" << std::endl;
}


int main() {
    return TestListenToDevicesPlugUnplug();
    //return TestListenToDevicesPlugUnplug_sdDevice();
}