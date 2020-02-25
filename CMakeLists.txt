cmake_minimum_required(VERSION 3.9)
project(demostats VERSION 0.1.0)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Protobuf REQUIRED)
add_subdirectory(messages)
add_subdirectory(kaitai)

set_property(SOURCE main.cpp APPEND PROPERTY OBJECT_DEPENDS ${KAITAI_OUTPUT})
set(CMAKE_CXX_STANDARD 14)


set(SOURCES src/main.cpp)
set(HEADERS )

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${ProtobufIncludePath})
include_directories(${KaitaiIncludePath})
include_directories(${KaitaiRuntimeIncludePath})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${KaitaiRuntimeSource})

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)
target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE
        -DKS_STR_ENCODING_NONE
)
target_link_libraries(
        ${PROJECT_NAME}
        messages
        kaitai
        ${Iconv_LIBRARIES}
)
