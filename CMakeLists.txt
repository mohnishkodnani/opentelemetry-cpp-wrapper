cmake_minimum_required(VERSION 3.22.1)

project(MetricsLib CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()
add_library(common INTERFACE)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/common/include)
# add_subdirectory(common)
# add_subdirectory(prometheus)
# add_subdirectory(opentelemetry)
# target_link_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/prometheus)
#add_subdirectory(prometheus)
add_subdirectory(opentelemetry)
