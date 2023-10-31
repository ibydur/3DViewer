enable_testing()

set(TEST_HEADER_FILES 
    ${CMAKE_SOURCE_DIR}/3DViewer/Geometry/include/CgalApi.h
)

set(TEST_SOURCE_FILES
    CgalApi_test.cpp
    ${CMAKE_SOURCE_DIR}/3DViewer/Geometry/src/CgalApi.cpp
)

add_executable(${APP_TARGET_NAME}_tests ${TEST_HEADER_FILES} ${TEST_SOURCE_FILES})

target_link_libraries(${APP_TARGET_NAME}_tests Qt5::Core Qt5::Gui CGAL::CGAL Qt5::Test)

target_include_directories(${APP_TARGET_NAME}_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/3DViewer/Geometry/include
)

add_test(NAME CgalApiTest COMMAND ${APP_TARGET_NAME}_tests)

# Copy the .objs to the build directory
add_custom_command(TARGET ${APP_TARGET_NAME}_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_SOURCE_DIR}/resources/objects/tmp.obj"
    "${CMAKE_SOURCE_DIR}/resources/objects/FinalBaseMesh.obj"
    "${CMAKE_CURRENT_BINARY_DIR}/"
)