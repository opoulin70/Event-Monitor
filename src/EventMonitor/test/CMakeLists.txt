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
        Device.test.cpp
        main.test.cpp
    )

    # Compiler options
    target_compile_options(TestDeviceMonitor PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_features(TestDeviceMonitor PUBLIC cxx_std_17)

    # Set include directories
    # target_include_directories(DeviceMonitorTests PRIVATE ${googletest_SOURCE_DIR}/googletest/include)

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