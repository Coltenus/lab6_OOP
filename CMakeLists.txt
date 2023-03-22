cmake_minimum_required(VERSION 3.24)
project(lab6)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab6 main.cpp src/file_objects/FObject.h src/file_objects/Directory.cpp src/file_objects/Directory.h src/file_objects/File.cpp src/file_objects/File.h src/utils/LException.h src/Filesystem.cpp src/Filesystem.h src/file_objects/file_objects.h)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif()