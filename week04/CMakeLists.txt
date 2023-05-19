cmake_minimum_required(VERSION 3.24)
project(week04)

set(OpenGlLinkers -lglut -lGLU -lGL)

add_executable(glut8 spin.cc )
target_link_libraries(glut8 ${OpenGlLinkers})