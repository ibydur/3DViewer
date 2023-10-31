find_package(Qt5 COMPONENTS Test REQUIRED)
enable_testing()

set(TEST_HEADER_FILES 
    ${CMAKE_SOURCE_DIR}/3DViewer/Geometry/include/CgalApi.h
)

set(TEST_SOURCE_FILES
    Cgal_api_test.cpp
    ${CMAKE_SOURCE_DIR}/3DViewer/Geometry/src/CgalApi.cpp
)

add_executable(${APP_TARGET_NAME}_tests ${TEST_HEADER_FILES} ${TEST_SOURCE_FILES})

target_link_libraries(${APP_TARGET_NAME}_tests Qt5::Test Qt5::Core Qt5::Gui CGAL::CGAL)

target_include_directories(${APP_TARGET_NAME}_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/3DViewer/Geometry/include
)

# Copy the design to the build directory
add_custom_command(TARGET ${APP_TARGET_NAME}_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/resources/Objects/tmp.obj"
    "${CMAKE_SOURCE_DIR}/resources/Objects/FinalBaseMesh.obj"
    "${CMAKE_CURRENT_BINARY_DIR}/"
)
