#include "TestMonitor.h"
#include <iostream>

void DisplayTestName (const std::string& testName) {
    std::cout << "*********************************" << std::endl;
    std::cout << "Testing " << testName << "..." << std::endl;
    std::cout << "*********************************" << std::endl;
}

int main() {
    DisplayTestName("TestCreateDeviceFromStaticSyspath");
    TestCreateDeviceFromStaticSyspath();

    DisplayTestName("TestCreateDeviceFromStaticSyspath");
    TestCreateDevicesFromSyspaths();

    DisplayTestName("TestEnumeratorAndDeviceRef");
    TestEnumeratorAndDeviceRef();
    
    return 0;
}