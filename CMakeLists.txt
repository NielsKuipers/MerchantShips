cmake_minimum_required(VERSION 3.20)
project(MerchantShips)

set(CMAKE_CXX_STANDARD 17)


add_library(MerchantShips-common STATIC
        src/Game.cpp include/game/RandomGeneration.h include/game/DBManager.h src/ship/Ship.cpp src/harbor/Harbor.cpp include/interface/Place.h include/game/menuHandler.h)

target_include_directories(MerchantShips-common PRIVATE
        src
        PUBLIC
        include)

add_executable(MerchantShips src/main.cpp include/sqlite/sqlite3.c include/harbor/HarborStates.h include/ship/CanonTypes.h)

target_link_libraries(MerchantShips PRIVATE MerchantShips-common)
