cmake_minimum_required(VERSION 3.15)

project(hft-itch-parser)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  readerwriterqueue
  GIT_REPOSITORY    https://github.com/cameron314/readerwriterqueue
  GIT_TAG           master
)
FetchContent_MakeAvailable(readerwriterqueue)

find_package(pcapplusplus)

add_executable(${PROJECT_NAME} 
    main.cpp
    inc/itch/itch_parser.cpp
)

set(HEADERS
    "${PROJECT_SOURCE_DIR}/inc/"
    "${PROJECT_SOURCE_DIR}/filesystem/"
    "${PROJECT_SOURCE_DIR}/itch/"
    "${PROJECT_SOURCE_DIR}/util/"
)

target_include_directories(${PROJECT_NAME} 
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${HEADERS}
)

target_link_libraries(${PROJECT_NAME}
    pcapplusplus::pcapplusplus
    readerwriterqueue
)