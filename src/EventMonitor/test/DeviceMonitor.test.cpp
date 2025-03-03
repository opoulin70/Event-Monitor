#include <gtest/gtest.h>
#include <EventMonitor/DeviceMonitor.h>

class DeviceMonitorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // ...
    }

    void TearDown() override {
        // ...
    }
};

TEST_F(DeviceMonitorTest, MultipleDeviceMonitorToOneEvent) {
    auto event = std::make_shared<Event>();

    DeviceMonitor monitor1 = DeviceMonitor(event);
    DeviceMonitor monitor2 = DeviceMonitor(event);
    
    // TODO
}

TEST_F(DeviceMonitorTest, SharedOwnershipBetweenDeviceMonitorsOfEvent_CheckEventReferenceCount) {
    // Step 1: Create a shared_ptr<Event> and check initial use_count.
    std::shared_ptr<Event> event = std::make_shared<Event>();
    EXPECT_EQ(event.use_count(), 1);

    // Step 2: Create first DeviceMonitor from Event instance.
    DeviceMonitor monitor1(event);
    EXPECT_EQ(monitor1.GetEvent().use_count(), 2) << "After creating monitor1 from existing event, reference count should now be 2.";

    // Step 3: Create second DeviceMonitor and attach it to the same event.
    DeviceMonitor monitor2 = DeviceMonitor();
    monitor2.AttachToEvent(event);
    EXPECT_EQ(monitor2.GetEvent().use_count(), 3) << "Two DeviceMonitors and the original Event instance should share ownership.";

    // Step 4: Create monitor3 and check count, then destroy and check again.
    {
        DeviceMonitor monitor3 = DeviceMonitor();
        monitor3.AttachToEvent(monitor2.GetEvent());
        EXPECT_EQ(event.use_count(), 4) << "Three DeviceMonitors and the original Event instance should share ownership.";
    }
    EXPECT_EQ(event.use_count(), 3) << "After monitor3 destruction, count should be 3 (Two DeviceMonitors and original Event instance).";

    // Step 5: Detach monitor2 from event and check count.
    monitor2.DetachFromEvent();
    EXPECT_EQ(monitor2.GetEvent(), nullptr);
    EXPECT_EQ(event.use_count(), 2) << "After detaching monitor2 from event, count should be 2 (monitor1 and Event instance).";

    // Step 6: Detach monitor1 from event two times, second time should work but do nothing.
    EXPECT_EQ(monitor1.IsAttachedToEvent(), true);
    monitor1.DetachFromEvent();
    EXPECT_EQ(monitor1.IsAttachedToEvent(), false);
    EXPECT_EQ(event.use_count(), 1) << "After detaching monitor1 from event, count should be 1 (only original Event instance left).";

    EXPECT_EQ(monitor1.IsAttachedToEvent(), false);
    monitor1.DetachFromEvent();
    EXPECT_EQ(monitor1.IsAttachedToEvent(), false);
    EXPECT_EQ(event.use_count(), 1) << "Detaching monitor1 a second time shouldn't do anything.";

    // Step 7: Finally destroy the last reference
    event.reset();
    EXPECT_EQ(event.use_count(), 0) << "After resetting the last reference, count should be 0.";
}

// TODO : Should not be able to start event loop without having attached it to a device monitor.