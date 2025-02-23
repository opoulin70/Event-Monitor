#include <EventMonitor/DeviceEnumerator.h>
#include "gtest/gtest.h"

class DeviceEnumeratorTests : public ::testing::Test {
protected:
    // TODO : Maybe this should be handled better.
    DeviceEnumeratorTests() : sdEnumerator(nullptr, &sd_device_enumerator_unref) {
        sd_device_enumerator* enumeratorTemp = nullptr;
        if (sd_device_enumerator_new(&enumeratorTemp) < 0 || !enumeratorTemp) {
            throw std::runtime_error("Invalid sd_device_enumerator* in DeviceEnumeratorTests constructor!");
        }
        sdEnumerator.reset(enumeratorTemp);
    }

    void SetUp() override {
        // TODO : Unnecessary ?
        ASSERT_TRUE(sdEnumerator);
    }

    void TearDown() override {
        // ...
    }

    std::unique_ptr<sd_device_enumerator, decltype(&sd_device_enumerator_unref)> sdEnumerator;
};

TEST_F(DeviceEnumeratorTests, GetFirstDeviceAndAllGetters) {
    // Create our DeviceEnumerator
    DeviceEnumerator enumerator = DeviceEnumerator();

    // Test getting first device, both should have the same device.
    sd_device* sdDev = sd_device_enumerator_get_device_first(sdEnumerator.get());
    auto dev = enumerator.GetDeviceFirst();

    if (sdDev) {
        EXPECT_TRUE(dev.has_value());

        // TODO : Test the different getters.
    }
}