find_package(Threads REQUIRED)
hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)
find_package(GMock CONFIG REQUIRED)
include(GoogleTest)

include_directories(. "../src")
set(TEST_SOURCE_FILES
        test_main.cpp)
add_executable(tests test_runner.cpp ${TEST_SOURCE_FILES})

target_link_libraries(tests ${PROJECT_NAME}_static)
target_link_libraries(tests GTest::gtest)
target_link_libraries(tests GMock::main)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(tests ${LIBCXX_LIBRARY})
endif()

gtest_add_tests(tests SOURCES ${TEST_SOURCE_FILES})

install(TARGETS tests DESTINATION bin)
