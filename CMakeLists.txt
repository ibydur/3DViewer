cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

project(3DViewer LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(UNIX)
    if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
        set(VCPKG_TARGET_TRIPLET "x64-osx")
    else()
        set(VCPKG_TARGET_TRIPLET "x64-linux")
    endif()
elseif(WIN32)
    set(VCPKG_TARGET_TRIPLET "x64-windows")
endif()

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/${VCPKG_TARGET_TRIPLET}" ${CMAKE_PREFIX_PATH})

set(Qt5Core_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/${VCPKG_TARGET_TRIPLET}/share/cmake/Qt5Core")
set(Qt5_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/${VCPKG_TARGET_TRIPLET}/share/cmake/Qt5")

find_package(Qt5 COMPONENTS Widgets Gui OpenGL Concurrent REQUIRED)
message(STATUS "Qt version found: ${Qt5_VERSION}")
find_package(CGAL REQUIRED)

set(APP_TARGET_NAME "3DViewer")

add_subdirectory(3DViewer)
add_subdirectory(tests)