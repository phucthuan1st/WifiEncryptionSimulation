cmake_minimum_required(VERSION 3.5)

project(WifiEncryptionSimulation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Find wxWidgets library
if(WIN32)
    # On Windows, add the path to the Cygwin binaries
    set(CMAKE_FIND_ROOT_PATH "C:/cygwin64/")
    # Explicitly set the generator and target architecture for Visual Studio on Windows
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(CMAKE_GENERATOR_TOOLSET "host=x64")
    endif()
    set(CMAKE_GENERATOR "Visual Studio")
endif()

# Find wxWidgets library
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/WEP)
include_directories(${CMAKE_SOURCE_DIR}/AES-CCMP)
include_directories(${CMAKE_SOURCE_DIR}/Component)

# Add subdirectories for Server and Client targets
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Server)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/Client)
