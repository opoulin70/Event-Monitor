#include <gtest/gtest.h>
#include <EventMonitor/DeviceEnumerator.h>

class DeviceEnumeratorTests : public ::testing::Test {
protected:
    DeviceEnumeratorTests() : sdEnumerator(nullptr, &sd_device_enumerator_unref) {}

    void SetUp() override {
        // Create a clean sd_device_enumerator between each tests.
        sd_device_enumerator* enumeratorTemp = nullptr;
        ASSERT_TRUE(sd_device_enumerator_new(&enumeratorTemp) >= 0 && enumeratorTemp) 
        << "Failed to create a sd_device_enumerator in DeviceEnumeratorTests SetUp!";
        sdEnumerator.reset(enumeratorTemp);
    }

    void TearDown() override {
        // ...
    }

    std::unique_ptr<sd_device_enumerator, decltype(&sd_device_enumerator_unref)> sdEnumerator;
};

// TODO : Make generalist test function for getters.
TEST_F(DeviceEnumeratorTests, GetFirstDeviceAndTestAllGetters) {
    // Create our DeviceEnumerator
    const DeviceEnumerator enumerator = DeviceEnumerator();

    // Try to get the first device, otherwise nothing to test.
    sd_device* sdDev = sd_device_enumerator_get_device_first(sdEnumerator.get());  
    if (sdDev) {
        // We got the first device, so we should be able to get the first device from our own enumerator.
        const auto dev = enumerator.GetDeviceFirst();
        ASSERT_TRUE(dev.has_value()) << "Failed to get first device from DeviceEnumerator while it exists!";
   
        // Test all our getters.
        // We first try getting the value from the device itself. 
        // If it succeeds, we expect the Device instance to return the same value.
        // If it fails, we expect the Device instance to return std::nullopt.
        const char* ret = nullptr;
        
        // Test GetDevname()
        if (sd_device_get_devname(sdDev, &ret) >= 0 && ret) { 
            ASSERT_TRUE(dev.value().GetDevname().has_value()) << "DeviceEnumerator GetDevname() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev.value().GetDevname().value(), std::string(ret)) << "Mismatch between systemd devname and DeviceEnumerator devname.";
        } 
        else {
            EXPECT_FALSE(dev.value().GetDevname().has_value())<< "DeviceEnumerator GetDevname() returned a value when systemd API did not.";
        }
        
        // Test GetDevpath()
        if (sd_device_get_devpath(sdDev, &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetDevpath().has_value()) << "DeviceEnumerator GetDevpath() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetDevpath().value(), std::string(ret)) << "Mismatch between systemd devpath and DeviceEnumerator devpath.";
        } 
        else {
            EXPECT_FALSE(dev->GetDevpath().has_value()) << "DeviceEnumerator GetDevpath() returned a value when systemd API did not.";
        }

        // Test GetDevtype()
        if (sd_device_get_devtype(sdDev, &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetDevtype().has_value()) << "DeviceEnumerator GetDevtype() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetDevtype().value(), std::string(ret)) << "Mismatch between systemd devtype and DeviceEnumerator devtype.";
        } 
        else {
            EXPECT_FALSE(dev->GetDevtype().has_value()) << "DeviceEnumerator GetDevtype() returned a value when systemd API did not.";
        }

        // Test GetDriver()
        if (sd_device_get_driver(sdDev, &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetDriver().has_value()) << "DeviceEnumerator GetDriver() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetDriver().value(), std::string(ret)) << "Mismatch between systemd driver and DeviceEnumerator driver.";
        } 
        else {
            EXPECT_FALSE(dev->GetDriver().has_value()) << "DeviceEnumerator GetDriver() returned a value when systemd API did not.";
        }

        // Test GetName()
        if (sd_device_get_sysname(sdDev, &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetName().has_value()) << "DeviceEnumerator GetName() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetName().value(), std::string(ret)) << "Mismatch between systemd name and DeviceEnumerator name.";
        } 
        else {
            EXPECT_FALSE(dev->GetName().has_value()) << "DeviceEnumerator GetName() returned a value when systemd API did not.";
        }

        // Test GetPath()
        if (sd_device_get_devpath(sdDev, &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetPath().has_value()) << "DeviceEnumerator GetPath() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetPath().value(), std::string(ret)) << "Mismatch between systemd syspath and DeviceEnumerator path.";
        } 
        else {
            EXPECT_FALSE(dev->GetPath().has_value()) << "DeviceEnumerator GetPath() returned a value when systemd API did not.";
        }

        // Test GetProductID()
        if (sd_device_get_property_value(sdDev, "ID_MODEL_ID", &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetProductID().has_value()) << "DeviceEnumerator GetProductID() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetProductID().value(), std::string(ret)) << "Mismatch between systemd ID_MODEL_ID and DeviceEnumerator ProductID.";
        } 
        else {
            EXPECT_FALSE(dev->GetProductID().has_value()) << "DeviceEnumerator GetProductID() returned a value when systemd API did not.";
        }

        // Test GetSerial()
        if (sd_device_get_property_value(sdDev, "ID_SERIAL", &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetSerial().has_value()) << "DeviceEnumerator GetSerial() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetSerial().value(), std::string(ret)) << "Mismatch between systemd ID_SERIAL_SHORT and DeviceEnumerator Serial.";
        } 
        else {
            EXPECT_FALSE(dev->GetSerial().has_value()) << "DeviceEnumerator GetSerial() returned a value when systemd API did not.";
        }

        // Test GetSubsystem()
        if (sd_device_get_subsystem(sdDev, &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetSubsystem().has_value()) << "DeviceEnumerator GetSubsystem() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetSubsystem().value(), std::string(ret)) << "Mismatch between systemd subsystem and DeviceEnumerator Subsystem.";
        } 
        else {
            EXPECT_FALSE(dev->GetSubsystem().has_value()) << "DeviceEnumerator GetSubsystem() returned a value when systemd API did not.";
        }

        // Test GetSysname()
        if (sd_device_get_sysname(sdDev, &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetSysname().has_value()) << "DeviceEnumerator GetSysname() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetSysname().value(), std::string(ret)) << "Mismatch between systemd sysname and DeviceEnumerator Sysname.";
        } 
        else {
            EXPECT_FALSE(dev->GetSysname().has_value()) << "DeviceEnumerator GetSysname() returned a value when systemd API did not.";
        }

        // Test GetSysnum()
        if (sd_device_get_sysnum(sdDev, &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetSysnum().has_value()) << "DeviceEnumerator GetSysnum() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetSysnum().value(), std::string(ret)) << "Mismatch between systemd sysnum and DeviceEnumerator Sysnum.";
        } 
        else {
            EXPECT_FALSE(dev->GetSysnum().has_value()) << "DeviceEnumerator GetSysnum() returned a value when systemd API did not.";
        }

        // Test GetSyspath()
        if (sd_device_get_syspath(sdDev, &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetSyspath().has_value()) << "DeviceEnumerator GetSyspath() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetSyspath().value(), std::string(ret)) << "Mismatch between systemd syspath and DeviceEnumerator Syspath.";
        } 
        else {
            EXPECT_FALSE(dev->GetSyspath().has_value()) << "DeviceEnumerator GetSyspath() returned a value when systemd API did not.";
        }
        
        // Test GetType()
        if (sd_device_get_devtype(sdDev, &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetType().has_value()) << "DeviceEnumerator GetType() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetType().value(), std::string(ret)) << "Mismatch between systemd devtype and DeviceEnumerator Type.";
        } 
        else {
            EXPECT_FALSE(dev->GetType().has_value()) << "DeviceEnumerator GetType() returned a value when systemd API did not.";
        }
        
        // Test GetVendorID()
        if (sd_device_get_property_value(sdDev, "ID_VENDOR_ID", &ret) >= 0 && ret) {
            ASSERT_TRUE(dev->GetVendorID().has_value()) << "DeviceEnumerator GetVendorID() returned nullopt while systemd API has a value.";
            EXPECT_EQ(dev->GetVendorID().value(), std::string(ret)) << "Mismatch between systemd ID_VENDOR_ID and DeviceEnumerator VendorID.";
        } 
        else {
            EXPECT_FALSE(dev->GetVendorID().has_value()) << "DeviceEnumerator GetVendorID() returned a value when systemd API did not.";
        }
    }
}

TEST_F(DeviceEnumeratorTests, GetAllDevices) {
        const DeviceEnumerator enumerator = DeviceEnumerator();

        // Get all devices from our own API.
        const auto devices = enumerator.GetAllDevices();

        // Get all devices from systemd API,
        std::vector<const sd_device*> sdDevices;
        for (const sd_device* dev = sd_device_enumerator_get_device_first(sdEnumerator.get()); 
        dev != nullptr;
        dev = sd_device_enumerator_get_device_next(sdEnumerator.get())) {
            sdDevices.push_back(dev);
        }
        
        EXPECT_EQ(devices.size(), sdDevices.size()) << "Mismatch between DeviceEnumerator and systemd API device counts.";
}

TEST_F(DeviceEnumeratorTests, GetAllDevices_WithSubsystemFilter) {
    DeviceEnumerator enumerator = DeviceEnumerator();

    // Add subsystem filter to our enumerator and get all devices.
    enumerator.AddMatchSubsystem("usb", true);
    const auto devices = enumerator.GetAllDevices();

    // Add a subsystem filter to the sd enumerator and get all devices.
    ASSERT_TRUE(sd_device_enumerator_add_match_subsystem(sdEnumerator.get(), "usb", 1) >= 0) 
    << "Failed to add a match subsystem filter for usb to systemd API enumerator.";

    std::vector<const sd_device*> sdDevices;
    for (const sd_device* dev = sd_device_enumerator_get_device_first(sdEnumerator.get()); 
    dev != nullptr;
    dev = sd_device_enumerator_get_device_next(sdEnumerator.get())) {
        sdDevices.push_back(dev);
    }
    
    EXPECT_EQ(devices.size(), sdDevices.size()) << "Mismatch between DeviceEnumerator and systemd API device counts.";
}

TEST_F(DeviceEnumeratorTests, AddSubsystemFilterAndRemove) {
    DeviceEnumerator enumerator = DeviceEnumerator();

    // Add usb filter and get count.
    enumerator.AddMatchSysname("usb");
    const auto usbDevicesCount = enumerator.GetAllDevices().size();

    // Remove the filter and compare.
    enumerator.Reset();
    const auto allDevicesCount = enumerator.GetAllDevices().size();
    EXPECT_NE(usbDevicesCount, allDevicesCount) << "Wrong DeviceEnumerator device count after removing the filter.";

    // Add usb filter again and compare.
    enumerator.AddMatchSysname("usb");
    const auto usbDevicesCount2 = enumerator.GetAllDevices().size();
    EXPECT_EQ(usbDevicesCount, usbDevicesCount2) << "Wrong DeviceEnumerator device count after removing and re-adding the filter.";
}