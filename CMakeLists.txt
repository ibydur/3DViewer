cmake_minimum_required(VERSION 3.15)
project(3DViewer LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Specify the Qt5 installation directory
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows" ${CMAKE_PREFIX_PATH})

set(QT_CUSTOM_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/share")
set(Qt5Core_DIR "${QT_CUSTOM_PATH}/cmake/Qt5Core")
set(Qt5_DIR "${QT_CUSTOM_PATH}/cmake/Qt5")

find_package(Qt5 COMPONENTS Widgets Gui OpenGL REQUIRED)
message(STATUS "Qt version found: ${Qt5_VERSION}")

# Add your source files
set(APP_TARGET_NAME "3DViewer")
add_executable(${APP_TARGET_NAME}  
        3DViewer/main.cpp 
        3DViewer/UI/src/3DViewer.cpp
        3DViewer/UI/include/3DViewer.h
        )

# Link the Qt5 libraries to your executable
target_link_libraries(${APP_TARGET_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
