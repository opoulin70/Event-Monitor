# Find systemd library (for sd-device)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SYSTEMD REQUIRED libsystemd)

# Create a library for the core EventMonitor functionality
add_library(LibEventMonitor 
Device.cpp 
DeviceEnumerator.cpp
DeviceMonitor.cpp
Event.cpp
TestMonitor.cpp
)

target_compile_options(LibEventMonitor PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(LibEventMonitor PUBLIC cxx_std_17)

# Set include directories (PUBLIC so it's available to dependents)
target_include_directories(LibEventMonitor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Include systemd headers
target_include_directories(LibEventMonitor PRIVATE ${SYSTEMD_INCLUDE_DIRS})

# Link systemd library
target_link_libraries(LibEventMonitor PRIVATE ${SYSTEMD_LDFLAGS})

# Add source files for main application
if (BUILD_MAIN_EXECUTABLE)
    add_executable(EventMonitor 
    main.cpp
    )

    target_link_libraries(EventMonitor LibEventMonitor)

    target_compile_options(EventMonitor PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (ENABLE_TESTS)
    add_subdirectory(test)
    target_compile_definitions(LibEventMonitor PRIVATE ENABLE_TESTS=1)  # Define ENABLE_TESTS macro
endif()