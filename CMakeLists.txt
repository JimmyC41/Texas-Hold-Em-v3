cmake_minimum_required(VERSION 3.10)

project(PokerTesting VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_FILES
    src/Board.cpp
    src/Card.cpp
    src/Dealer.cpp
    src/Deck.cpp
    src/Player.cpp
)

add_library(PokerLib ${SRC_FILES})

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(DealTest test/DealTest.cpp)

target_link_libraries(DealTest PokerLib GTest::GTest GTest::Main pthread)

add_test(NAME DealTest COMMAND DealTest)
