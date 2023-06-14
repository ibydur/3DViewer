cmake_minimum_required(VERSION 3.0)  # Minimum required CMake version

project(3DViewer) # Name of your project

# Add your source files
add_executable(3DViewer 
    3DViewer/src/main.cpp
)

# Additional configuration options, such as compiler flags or libraries
# target_compile_options(MyExecutable PRIVATE -Wall -Wextra)
# target_link_libraries(MyExecutable PRIVATE mylib)