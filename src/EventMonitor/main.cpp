#include <EventMonitor/TestMonitor.h>
#include <iostream>

void DisplayTestName (const std::string& testName) {
    std::cout << "*********************************" << std::endl;
    std::cout << "Testing " << testName << "..." << std::endl;
    std::cout << "*********************************" << std::endl;
}

int main() {
    DisplayTestName("TestCreateDeviceFromStaticSyspath");
    TestCreateDevicesFromSyspaths();
    
    return 0;
}