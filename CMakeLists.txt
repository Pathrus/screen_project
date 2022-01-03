cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME training)
project(${PROJECT_NAME} CXX)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(WARNING "Not recommended to build source in the main folder\n"
                    "Use build or your own folder.")
endif()

enable_language(CXX)
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")



#set(CMAKE_BUILD_TYPE Release)


#

add_executable(screen_communication ${PROJECT_SOURCE_DIR}/App/screen_communication.cpp)

add_subdirectory(Library)
include_directories(${PROJECT_SOURCE_DIR}/Library)

find_package(Threads REQUIRED)

target_link_libraries(screen_communication screen ${CMAKE_THREAD_LIBS_INIT})

#find_package()





