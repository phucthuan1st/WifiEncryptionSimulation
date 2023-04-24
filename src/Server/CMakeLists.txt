project(Server)

include_directories(${CMAKE_SOURCE_DIR}/WEP)
include_directories(${CMAKE_SOURCE_DIR}/AES-CCMP)
include_directories(${CMAKE_SOURCE_DIR}/Component)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(Server ${CMAKE_SOURCE_DIR}/src/Server/main.cpp)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(Server
    ${wxWidgets_LIBRARIES}
    wx_gtk3u_adv-3.0
    wx_gtk3u_core-3.0
    wx_baseu-3.0
)