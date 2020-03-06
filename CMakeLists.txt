cmake_minimum_required(VERSION 3.9)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include("${CMAKE_MODULE_PATH}/HunterGate.cmake")
# Enable ExternalProject CMake module
HunterGate(
        URL "https://github.com/ruslo/hunter/archive/v0.23.11.tar.gz"
        SHA1 "b732407d782459f89950ab52bc0ff9612f3e4be2"
)

project(demostats VERSION 2.0.0)
set(CMAKE_CXX_STANDARD 14)

include(CTest)
find_package(Protobuf REQUIRED)
add_subdirectory(messages)

# custom options for MacOS
if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif()

set(SOURCES src/main.cpp src/hl2demo/dstream.cpp src/hl2demo/frame.cpp src/hl2demo/message.cpp src/hl2demo/string_table_update.cpp)
set(HEADERS src/hl2demo/dstream.h src/hl2demo/header.h src/hl2demo/frame.h src/hl2demo/message.h src/hl2demo/string_table_update.h)
configure_file(src/version.h.in ${CMAKE_BINARY_DIR}/generated/version.h)

include_directories(${CMAKE_BINARY_DIR}/generated/)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${ProtobufIncludePath})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
# include the protobuf messages
target_link_libraries(${PROJECT_NAME} messages)

enable_testing()
# compile static version for testing
add_library(${PROJECT_NAME}_static STATIC ${HEADERS} ${SOURCES})
add_subdirectory(tests)