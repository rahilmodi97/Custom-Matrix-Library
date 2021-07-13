cmake_minimum_required(VERSION 2.8)
project(matmul)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(INCLUDE "${CMAKE_CURRENT_LIST_DIR}/../include/")
set(SOURCE_FILE main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})
target_include_directories(${PROJECT_NAME}
PUBLIC
${INCLUDE})