
set(HEADER_FILES
    UI/include/3DViewer.h
    Renderer/include/OpenGLRenderer.h
    Renderer/include/Camera.h
    Geometry/include/CgalApi.h
    Scene/include/SceneObject.h
    Scene/include/Scene.h
)

set(SOURCE_FILES
    main.cpp
    UI/src/3DViewer.cpp
    Renderer/src/OpenGLRenderer.cpp
    Renderer/src/Camera.cpp
    Geometry/src/CgalApi.cpp
    Scene/src/Scene.cpp
    Scene/src/SceneObject.cpp
)

add_executable(${APP_TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${APP_TARGET_NAME} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL Qt5::Concurrent CGAL::CGAL)

target_include_directories(${APP_TARGET_NAME} PRIVATE
    UI/include
    Renderer/include
    Geometry/include
    Scene/include
)