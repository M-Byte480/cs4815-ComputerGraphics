cmake_minimum_required(VERSION 3.24)
project(week06)

set(OpenGlLinkers -lglut -lGLU -lGL)

add_executable(bezier bezier.cc)
target_link_libraries(bezier ${OpenGlLinkers})