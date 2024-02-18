cmake_minimum_required(VERSION 3.21)

project("WebServer" VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WS_TESTS_TARGET "ws_tests")

find_package(Boost 1.82 REQUIRED 
    COMPONENTS 
        system
        program_options)
find_package(OpenSSL REQUIRED)
find_package(GTest REQUIRED)

add_library(${PROJECT_NAME} STATIC)

add_executable(${WS_TESTS_TARGET}
    tests/main.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/web_server.cpp
        src/ws_options.cpp
        src/ws_logger.cpp
        src/request_handler.cpp
        src/ssl.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_include_directories(${WS_TESTS_TARGET} 
    PRIVATE 
        ${GTEST_INCLUDE_DIRS})

# without it public headers won't get installed
set(PUBLIC_HEADERS
    include/web_server.hpp
    include/ws_options.hpp
    include/ws_logger.hpp
)

set_target_properties(${PROJECT_NAME} 
    PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        ${Boost_LIBRARIES} 
        OpenSSL::Crypto
        OpenSSL::SSL)
target_link_libraries(${WS_TESTS_TARGET}
    PRIVATE 
        ${GTEST_BOTH_LIBRARIES} ${PROJECT_NAME})

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -pedantic)
    target_compile_options(${WS_TESTS_TARGET} PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Installing)

# tests
enable_testing()
add_test(NAME ${WS_TESTS_TARGET} COMMAND ${WS_TESTS_TARGET})

# test files after deploy
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})