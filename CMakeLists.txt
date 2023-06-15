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
add_executable(${APP_TARGET_NAME}  3DViewer/main.cpp)

# Link the Qt5 libraries to your executable
target_link_libraries(${APP_TARGET_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)

# Set the QT_QPA_PLATFORM_PLUGIN_PATH environment variable
set(QT_PLUGIN_PATH "${CMAKE_PREFIX_PATH}/plugins")

add_custom_command(TARGET ${APP_TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE:Qt5::Gui>
        $<TARGET_FILE:Qt5::Widgets>
        $<TARGET_FILE_DIR:${APP_TARGET_NAME}>)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${APP_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_PREFIX_PATH}/bin/brotlicommon.dll"
            "${CMAKE_PREFIX_PATH}/bin/brotlidec.dll"
            "${CMAKE_PREFIX_PATH}/bin/bz2d.dll"
            "${CMAKE_PREFIX_PATH}/bin/double-conversion.dll"
            "${CMAKE_PREFIX_PATH}/bin/freetyped.dll"
            "${CMAKE_PREFIX_PATH}/bin/libpng16d.dll"
            "${CMAKE_PREFIX_PATH}/bin/pcre2-16d.dll"
            "${CMAKE_PREFIX_PATH}/bin/zlibd1.dll"
            "${CMAKE_PREFIX_PATH}/bin/zstd.dll"
            $<TARGET_FILE_DIR:${APP_TARGET_NAME}>)
else()
    add_custom_command(TARGET ${APP_TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_PREFIX_PATH}/bin/brotlicommon.dll"
            "${CMAKE_PREFIX_PATH}/bin/brotlidec.dll"
            "${CMAKE_PREFIX_PATH}/bin/bz2.dll"
            "${CMAKE_PREFIX_PATH}/bin/double-conversion.dll"
            "${CMAKE_PREFIX_PATH}/bin/freetype.dll"
            "${CMAKE_PREFIX_PATH}/bin/libpng16.dll"
            "${CMAKE_PREFIX_PATH}/bin/pcre2-16.dll"
            "${CMAKE_PREFIX_PATH}/bin/zlib1.dll"
            "${CMAKE_PREFIX_PATH}/bin/zstd.dll"
            $<TARGET_FILE_DIR:${APP_TARGET_NAME}>)
endif()

add_custom_command(TARGET ${APP_TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_PREFIX_PATH}/plugins/platforms" 
            $<TARGET_FILE_DIR:${APP_TARGET_NAME}>/platforms)