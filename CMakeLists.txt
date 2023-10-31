cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

project(3DViewer LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(Qt5 COMPONENTS Widgets Gui OpenGL Concurrent Test REQUIRED)
find_package(CGAL REQUIRED)

set(APP_TARGET_NAME "3DViewer")

add_subdirectory(3DViewer)
add_subdirectory(tests)