cmake_minimum_required(VERSION 3.22.1)

project(OpenTelemetryUnitTests)

find_package(Catch2 3 REQUIRED)

# Set the parent include and source directories
# set(PARENT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include")
# set(PARENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src")

# Add the unit test executable
add_executable(OpenTelemetryUnitTests
    metric_tests.cpp
    # Add more test files here
)

# Link the Catch2 library
target_link_libraries(OpenTelemetryUnitTests PRIVATE Catch2::Catch2WithMain metrics_opentelemetry)
include(CTest)
include(Catch)
catch_discover_tests(OpenTelemetryUnitTests)