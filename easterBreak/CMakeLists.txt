cmake_minimum_required(VERSION 3.24)
project(easter)

set(OpenGlLinkers -lglut -lGLU -lGL)

add_executable(easter yfig.cc)
target_link_libraries(easter ${OpenGlLinkers})