project(Client)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
include_directories(${CMAKE_SOURCE_DIR}/WEP)
include_directories(${CMAKE_SOURCE_DIR}/AES-CCMP)

add_executable(Client ${CMAKE_SOURCE_DIR}/src/Client/main.cpp)
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(Client
    ${wxWidgets_LIBRARIES}
    wx_gtk3u_adv-3.0
    wx_gtk3u_core-3.0
    wx_baseu-3.0
)