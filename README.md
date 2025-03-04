# **Device Event Monitor**  
*A Modern C++ API for Device Event Management on Linux*  

## **Overview**  
Device Event Monitor is a modern C++ API designed to monitor hardware device events on Linux using `libsystemd` (specifically `sd-device` and `sd-event`). 
The goal of this project is to provide an efficient, high-level and easy to use interface for tracking devices, such as USB devices, and their interactions with the operating system while adhering to modern C++ best practices.  

The project is in active development and currently supports attaching to an event loop and handling device events through user-defined callbacks.  

---

## **Current Features**  
- **Device Monitoring API**: Initializes a `DeviceMonitor` that tracks device events using `libsystemd`.  
- **Event Loop Integration**: Uses `sd_event` from `systemd` to handle device events asynchronously.  
- **User-Defined Callbacks**: Supports setting a custom callback function to react to device events.  
- **Modern C++ Design**:  
  - Utilizes RAII for resource management through smart pointers.  
  - Encapsulates `libsystemd` details behind an easy-to-use API.  
- **Basic Device Detection**: Detects device interactions and triggers user-defined callback.  

---

## **Current Issues and Next Steps**  
### **Known Issues**  
- Error handling and logging need refinement.  
- Additional unit tests are needed to validate edge cases.  

### **Planned Improvements**  
- Implement correct device action retrieval for "add," "remove," and other actions.  
- Implement new features for Event class.
- Expand functionalities for all device types.
- Implement asynchronous polling for improved performance.  
- Improve API documentation and user guides.  

---

## **Installation and Usage**  

### **Prerequisites**  
- Linux (required due to the dependency on `libsystemd`).  
- C++ Compiler (`g++` or `clang++`, supporting C++17 or later).  
- CMake (for building the project).  
- `libsystemd-dev` (required API).

### **Build Instructions**  
```bash
git clone https://github.com/your-repo/DeviceEventMonitor.git
cd DeviceEventMonitor
mkdir build && cd build
cmake ..
make
```

### **Usage exemple** 

```cpp
#include <EventMonitor/DeviceMonitor.h>

int main() {
  // Create the event loop.
  auto eventLoop = std::make_shared<Event>();
  
  // Create the device monitor and attach it to the event loop.
  DeviceMonitor monitor(eventLoop);

  // Set callback to handle device events.
  monitor.SetCallback([](const DeviceMonitor&, Device device) {
      std::cout << "Device event detected: " << device.GetName().value_or("Unknown") << std::endl;
  });

  // Start monitoring and run the event loop.
  monitor.StartMonitoring();
  sd_event_loop(eventLoop->GetEvent());
  
  return 0;
}
```
