set(KaitaiIncludePath ${CMAKE_CURRENT_BINARY_DIR}
        CACHE INTERNAL "Path to generated kaitai files.")
set(KaitaiRuntimeIncludePath "${CMAKE_CURRENT_SOURCE_DIR}/kaitai-runtime"
        CACHE INTERNAL "Path to kaitai runtime files.")
set(KaitaiRuntimeSource "${CMAKE_CURRENT_SOURCE_DIR}/kaitai-runtime/kaitai/kaitaistream.cpp"
        CACHE INTERNAL "Path to kaitai runtime files.")

set(KAITAI_OUTPUT frame.cpp frame.h header.cpp header.h hl2demo.cpp hl2demo.h message.cpp message.h vlq_base128_le.cpp vlq_base128_le.h)

add_custom_command(
        OUTPUT ${KAITAI_OUTPUT}
        PRE_BUILD
        COMMAND kaitai-struct-compiler -t cpp_stl ${CMAKE_CURRENT_SOURCE_DIR}/hl2demo.ksy
        DEPENDS hl2demo.ksy frame.ksy header.ksy message.ksy vlq_base128_le.ksy
)
include_directories(${KaitaiRuntimeIncludePath})

add_library(kaitai STATIC ${KAITAI_OUTPUT})
