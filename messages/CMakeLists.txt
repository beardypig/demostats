set(CMAKE_CXX_STANDARD 11)

set(Protobuf_IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})
file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
add_library(messages STATIC ${ProtoSources} ${ProtoHeaders})
target_link_libraries(messages ${Protobuf_LIBRARIES})

set(ProtobufIncludePath ${CMAKE_CURRENT_BINARY_DIR}
        CACHE INTERNAL "Path to generated protobuf files.")