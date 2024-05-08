add_library(common INTERFACE)
target_sources(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/dummy.cpp)
target_include_directories(common INTERFACE SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)