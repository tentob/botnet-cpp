set(CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")

# Project configuration
project(botnet)
cmake_minimum_required(VERSION 2.8)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src" SRC_LIST)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/botnet" SRC_LIST)

# Build & Link
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} dl boost_program_options)

