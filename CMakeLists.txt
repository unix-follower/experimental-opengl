cmake_minimum_required(VERSION 4.4.1)

project(experimental_opengl VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED CONFIG)
find_package(glad REQUIRED CONFIG)
find_package(cxxopts REQUIRED)

add_library(experimental_opengl_core
	STATIC
	src/basic_shapes.hpp
	src/equilateral_triangle.cpp
	src/equilateral_triangle_with_inscribed_upside_down_eq_triangle.cpp
)
target_compile_features(experimental_opengl_core PUBLIC cxx_std_23)
target_link_libraries(experimental_opengl_core PUBLIC OpenGL::GL glfw glad::glad cxxopts::cxxopts)

add_executable(experimental_opengl src/main.cpp)
target_link_libraries(experimental_opengl PRIVATE experimental_opengl_core)
set_target_properties(experimental_opengl PROPERTIES OUTPUT_NAME experimental_opengl_app)

enable_testing()
