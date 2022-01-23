project(tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run ShipTests.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main)
target_link_libraries(Google_Tests_run MerchantShips-common)