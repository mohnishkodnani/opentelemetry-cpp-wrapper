# Add package for open telemetry cpp library using find_package
find_package(opentelemetry-cpp CONFIG REQUIRED)
find_package(prometheus-cpp REQUIRED) # Needed for prometheus exporter inside opentelemetry
find_package(benchmark REQUIRED) # Needed for benchmarking

file(GLOB SOURCES "src/*.cpp")

add_library(metrics_opentelemetry ${SOURCES})
target_include_directories(metrics_opentelemetry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENTELEMETRY_CPP_INCLUDE_DIRS})
target_link_libraries(metrics_opentelemetry PUBLIC common ${OPENTELEMETRY_CPP_LIBRARIES})
set_target_properties(metrics_opentelemetry PROPERTIES LINKER_LANGUAGE CXX)

add_executable(metrics_opentelemetry_exe ${SOURCES})
target_include_directories(metrics_opentelemetry_exe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENTELEMETRY_CPP_INCLUDE_DIRS})
target_link_libraries(metrics_opentelemetry_exe PUBLIC common ${OPENTELEMETRY_CPP_LIBRARIES})

# if(BUILD_TESTING)
    # Enable testing with CTest
    enable_testing()
    # Add the test subdirectory
    add_subdirectory(tests)
# endif()
