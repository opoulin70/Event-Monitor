cmake_minimum_required(VERSION 3.10)

# Project name
project(EventMonitor)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find systemd library (for sd-device)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SYSTEMD REQUIRED libsystemd)

# Add source files
add_executable(DeviceMonitor src/main.cpp src/TestMonitor.cpp src/Device.cpp src/DeviceEnumerator.cpp)

# Include systemd headers
target_include_directories(DeviceMonitor PRIVATE ${SYSTEMD_INCLUDE_DIRS})

# Link systemd library
target_link_libraries(DeviceMonitor PRIVATE ${SYSTEMD_LDFLAGS})

# Compiler warnings (optional, but useful for debugging)
target_compile_options(DeviceMonitor PRIVATE -Wall -Wextra -Wpedantic)