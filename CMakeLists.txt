cmake_minimum_required(VERSION 3.5)
project(WifiEncryptionSimulation)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Server)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Client)