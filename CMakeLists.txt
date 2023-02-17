cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME process-manager)

project(${PROJECT_NAME})

add_definitions(-DUSE_LOG_FILE)

if (BOARD STREQUAL MT9050)
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR arm)

    set(SYSROOT "${TOOLS_PATH}/usr/arm-buildroot-linux-gnueabihf/sysroot")

    set(CMAKE_C_COMPILER "${TOOLS_PATH}/usr/bin/arm-linux-gnueabihf-gcc")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${SYSROOT}/usr/include/glib-2.0  -I${SYSROOT}/usr/lib/glib-2.0/include -Wno-abi")

    set(CMAKE_CXX_COMPILER "${TOOLS_PATH}/usr/bin/arm-linux-gnueabihf-g++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${SYSROOT}/usr/include/glib-2.0  -I${SYSROOT}/usr/lib/glib-2.0/include -Wno-abi")

endif()

include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/Libraries/*"

    "${CMAKE_SOURCE_DIR}/Sources/*"

    "${CMAKE_SOURCE_DIR}/Controllers/*"

    "${CMAKE_SOURCE_DIR}/Session/*"

)

message(STATUS "BOARD: ${BOARD}")
message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME} ${SOURCES} main.cpp)

target_link_libraries(process-manager pthread glib-2.0 httpserver jsoncpp gobject-2.0 gio-2.0 wpa_client curl rt sqlite3 asound boost_system z)

