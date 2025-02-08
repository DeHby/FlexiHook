cmake_minimum_required(VERSION 3.10)

project(FlexiHook)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT /GL")

add_subdirectory("./third-party/minhook")

include_directories(
    "./src/FlexiHook"
)

link_directories("./third-party/lib")

aux_source_directory("./src/FlexiHook" SRCS)

add_executable(${PROJECT_NAME} "./src/main.cpp" ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE minhook)