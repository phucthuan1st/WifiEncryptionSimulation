cmake_minimum_required(VERSION 3.5)
project(Client)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/WEP)
include_directories(${CMAKE_SOURCE_DIR}/AES-CCMP)
include_directories(${CMAKE_SOURCE_DIR}/Component)

# Add the target
add_executable(Client ${CMAKE_SOURCE_DIR}/src/Client/main.cpp)

# Link the wxWidgets libraries
target_link_libraries(Client ${wxWidgets_LIBRARIES})
