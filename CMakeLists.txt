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
    src/PotManager.cpp
    src/TurnManager.cpp
    src/ActionManager.cpp
    src/GamePlayers.cpp
    src/ClientManager.cpp
    src/GameController.cpp
)

add_library(PokerLib ${SRC_FILES})

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(DealTest test/DealTest.cpp)
target_link_libraries(DealTest PRIVATE PokerLib GTest::GTest GTest::Main pthread)

add_executable(PotManagerTest test/PotManagerTest.cpp)
target_link_libraries(PotManagerTest PRIVATE PokerLib GTest::GTest GTest::Main pthread)

add_executable(TurnManagerTest test/TurnManagerTest.cpp)
target_link_libraries(TurnManagerTest PRIVATE PokerLib GTest::GTest GTest::Main pthread)

add_executable(ActionTest test/ActionTest.cpp)
target_link_libraries(ActionTest PRIVATE PokerLib GTest::GTest GTest::Main pthread)

add_executable(AllInActionTest test/AllInActionTest.cpp)
target_link_libraries(AllInActionTest PRIVATE PokerLib GTest::GTest GTest::Main pthread)

add_executable(GamePlayersTest test/GamePlayersTest.cpp)
target_link_libraries(GamePlayersTest PRIVATE PokerLib GTest::GTest GTest::Main pthread)

add_executable(ClientManagerTest test/ClientManagerTest.cpp)
target_link_libraries(ClientManagerTest PRIVATE PokerLib GTest::GTest GTest::Main pthread)

add_executable(GameControllerTest test/GameControllerTest.cpp)
target_link_libraries(GameControllerTest PRIVATE PokerLib GTest::GTest GTest::Main pthread)

add_test(NAME DealTest COMMAND DealTest)
add_test(NAME PotManagerTest COMMAND PotManagerTest)
add_test(NAME TurnManagerTest COMMAND TurnManagerTest)
add_test(NAME ActionTest COMMAND ActionTest)
add_test(NAME GamePlayersTest COMMAND GamePlayersTest)
add_test(NAME ClientManagerTest COMMAND ClientManagerTest)
add_test(NAME GameControllerTest COMMAND GameControllerTest)

