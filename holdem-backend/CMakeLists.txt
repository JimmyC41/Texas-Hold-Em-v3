cmake_minimum_required(VERSION 3.10)

project(PokerTests VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC_FILES
    src/*.cpp
)

add_library(PokerLib ${SRC_FILES})

# TESTS

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

function(addPokerTest TEST_NAME TEST_FILE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE PokerLib GTest::GTest GTest::Main pthread)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

set(TEST_FILES
    DealTest
    PotManagerTest
    TurnManagerTest
    ActionTest
    AllInActionTest
    GamePlayersTest
    AwardPotTest
    HandEvaluationTest
)

foreach(TEST_NAME IN LISTS TEST_FILES)
    addPokerTest(${TEST_NAME}  test/${TEST_NAME}.cpp)
endforeach()

# MAIN FUNCTION
add_executable(PokerV3 main.cpp)
target_link_libraries(PokerV3 PRIVATE PokerLib)

add_custom_target(run_tests
    COMMAND ctest --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running all tests..."
)