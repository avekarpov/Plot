cmake_minimum_required(VERSION 3.20)
get_filename_component(project_dir_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${project_dir_name})

set(CMAKE_CXX_STANDARD 17)

file(GLOB cpps ${PROJECT_SOURCE_DIR}/sources/*.cpp)
file(GLOB hpps ${PROJECT_SOURCE_DIR}/sources/*.hpp)

include_directories(C:/Libraries/SFML/include)
link_directories(C:/Libraries/SFML/lib/Debug)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${hpps} ${cpps})

target_link_libraries(${PROJECT_NAME} sfml-system-d.lib sfml-window-d.lib sfml-graphics-d.lib ${OPENGL_LIBRARIES})