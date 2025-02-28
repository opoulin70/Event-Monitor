#include <gtest/gtest.h>
#include <EventMonitor/DeviceMonitor.h>

TEST(DeviceMonitorTest, MultipleDeviceMonitorToOneEvent) {
    auto event = std::make_shared<Event>();

    DeviceMonitor monitor1 = DeviceMonitor(event);
    DeviceMonitor monitor2 = DeviceMonitor(event);
    
    // TODO
}

// TEST(DeviceMonitorTest, EventLoopReferenceCount) {
//     // Step 1: Create a shared_ptr<Event> and check initial use_count
//     std::shared_ptr<Event> event = std::make_shared<Event>();
//     EXPECT_EQ(event.use_count(), 1) << "Initial reference count should be 1.";

//     // Step 2: Create first DeviceMonitor (ownership should be moved)
//     DeviceMonitor monitor1(std::move(event));
//     EXPECT_EQ(monitor1.GetEventLoop().use_count(), 1) << "After moving, reference count should remain 1.";

//     // Step 3: Create second DeviceMonitor from the same shared_ptr
//     std::shared_ptr<Event> sharedEvent = monitor1.GetEventLoop();
//     DeviceMonitor monitor2(sharedEvent);
//     EXPECT_EQ(sharedEvent.use_count(), 2) << "Two DeviceMonitors should share ownership, count = 2.";

//     // Step 4: Destroy monitor2 and check reference count
//     {
//         DeviceMonitor monitor3(sharedEvent);
//         EXPECT_EQ(sharedEvent.use_count(), 3) << "Three DeviceMonitors should share ownership, count = 3.";
//     }
//     EXPECT_EQ(sharedEvent.use_count(), 2) << "After monitor3 destruction, count should be 2.";

//     // Step 5: Destroy monitor1 and check final reference count
//     {
//         std::shared_ptr<Event> tmpEvent = monitor1.GetEventLoop();
//     }  // tmpEvent goes out of scope here
//     EXPECT_EQ(sharedEvent.use_count(), 1) << "After monitor1 destruction, count should be 1.";

//     // Step 6: Destroy monitor2, ensuring all references are gone
//     {
//         std::shared_ptr<Event> tmpEvent = monitor2.GetEventLoop();
//     }
//     EXPECT_EQ(sharedEvent.use_count(), 1) << "After monitor2 destruction, last owner should remain.";

//     // Step 7: Finally destroy the last reference
//     sharedEvent.reset();
//     EXPECT_EQ(sharedEvent.use_count(), 0) << "After resetting the last reference, count should be 0.";
// }

// TODO : Should not be able to start event loop without having attached it to a device monitor.