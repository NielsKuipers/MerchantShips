cmake_minimum_required(VERSION 3.20)
project(MerchantShips)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES src/main.cpp include/sqlite/sqlite3.c)

add_library(MerchantShips-common STATIC
        src/Game.cpp include/Game.h include/RandomGeneration.h)

target_include_directories(MerchantShips-common PRIVATE
        src
        PUBLIC
        include)

add_executable(MerchantShips ${SOURCE_FILES})

target_link_libraries(MerchantShips PRIVATE MerchantShips-common)
