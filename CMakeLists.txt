cmake_minimum_required(VERSION 3.24)
project(lab6)

set(CMAKE_CXX_STANDARD 20)

find_package(Lua REQUIRED)
find_package(sol2 QUIET)

add_executable(lab6 main.cpp src/file_objects/FObject.h src/file_objects/Directory.cpp src/file_objects/Directory.h src/file_objects/File.cpp src/file_objects/File.h src/utils/LException.h src/Filesystem.cpp src/Filesystem.h src/file_objects/file_objects.h src/Scripts.cpp src/Scripts.h src/LuaFile.cpp src/LuaFile.h)

if(NOT sol2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            sol2
            GIT_REPOSITORY https://github.com/ThePhD/sol2.git
            GIT_TAG 8618e39486dde855d671e1e7ecc3f50b43aee257)
    FetchContent_MakeAvailable(sol2)
endif()

target_link_libraries(lab6 ${LUA_LIBRARIES} sol2)

target_include_directories(lab6 PUBLIC ${LUA_INCLUDE_DIR} sol2)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif()