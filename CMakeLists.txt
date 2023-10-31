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
find_package(CGAL REQUIRED)

message(STATUS "Qt version found: ${Qt5_VERSION}")

# Create variables for header and source files
set(HEADER_FILES
    3DViewer/UI/include/3DViewer.h
    3DViewer/Renderer/include/OpenGLRenderer.h
    3DViewer/Renderer/include/Camera.h
    3DViewer/Geometry/include/CgalApi.h
    3DViewer/Scene/include/SceneObject.h
    3DViewer/Scene/include/Scene.h
)

set(SOURCE_FILES
    3DViewer/main.cpp
    3DViewer/UI/src/3DViewer.cpp
    3DViewer/Renderer/src/OpenGLRenderer.cpp
    3DViewer/Renderer/src/Camera.cpp
    3DViewer/Geometry/src/CgalApi.cpp
    3DViewer/Scene/src/Scene.cpp
    3DViewer/Scene/src/SceneObject.cpp
)

# Add your source files
set(APP_TARGET_NAME "3DViewer")
add_executable(${APP_TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Link the Qt5 libraries to your executable
target_link_libraries(${APP_TARGET_NAME} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL Qt5::Concurrent CGAL::CGAL)

# Add include directories as paths for the target
target_include_directories(${APP_TARGET_NAME} PRIVATE
    3DViewer/UI/include
    3DViewer/Renderer/include
    3DViewer/Geometry/include
    3DViewer/Scene/include
)

add_subdirectory(tests)