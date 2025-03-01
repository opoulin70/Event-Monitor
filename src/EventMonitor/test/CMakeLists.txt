if (ENABLE_TESTS)
    # Fetch GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(googletest)

    # Add test executable
    add_executable(TestDeviceMonitor
        main.test.cpp
        Utilities.cpp
        Device.test.cpp
        DeviceEnumerator.test.cpp
        DeviceMonitor.test.cpp
    )

    # Compiler options
    target_compile_options(TestDeviceMonitor PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_features(TestDeviceMonitor PUBLIC cxx_std_17)
    target_compile_definitions(TestDeviceMonitor PRIVATE ENABLE_TESTS=1) # Define ENABLE_TESTS macro

    # Link Google Test and the application sources
    target_link_libraries(TestDeviceMonitor
        gtest_main
        gtest
        pthread  # Required on Linux for gtest
        LibEventMonitor
    )

    # Register tests with CTest
    include(GoogleTest)
    gtest_discover_tests(TestDeviceMonitor)
endif()