#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "ActionManager.h"
#include "Action.h"
#include "Actions/AllInBetAction.h"
#include "Actions/AllInCallAction.h"
#include "Actions/BetAction.h"
#include "Actions/BlindAction.h"
#include "Actions/CallAction.h"
#include "Actions/CheckAction.h"
#include "Actions/FoldAction.h"
#include "Actions/RaiseAction.h"
#include "Dealer.h"
#include "ClientManager.h"
#include "GamePlayers.h"
#include "PotManager.h"
#include "TurnManager.h"
#include "Deck.h"
#include "Board.h"
#include "HandEvaluator.h"
#include "StreetState.h"

#include <string>
#include <memory.h>
using namespace std;

class GameController {
private:
    size_t smallBlind;
    size_t bigBlind;
    int roundNum;

    Deck deck;
    Board board;
    Dealer dealer;
    GamePlayers gamePlayers;
    ActionManager actionManager;
    TurnManager turnManager;
    ClientManager clientManager;
    PotManager potManager;
    HandEvaluator handEvaluator;
    StreetState streetState;

    // Helper function to convert Street to string
    string streetToStr(Street street);

    // Helper function to deal hole cards to players
    void dealPlayers();

    // Helper function to deals community cards to board
    void dealBoard(int numCards);

    // Street Helper function to set up game state for a new street
    // TurnManager: Updates first player to act
    // PotManager: Handles blinds
    // Dealer: Deals cards to players / board
    // StreetState: Sets the street and the intial number of players in the hand
    // Called at the beginning of each street
    void setupStreet(Street newStreet);

    // Street Helper function to process a new player action
    // ActionManager: Add action to timeline and update action state
    // PotManager: Player bet is recorded in the player bets map
    // TurnManager: Player in hand status update based on action
    void processNewAction(shared_ptr<Player>& player, const shared_ptr<Action>& playerAction);

    // Street Helper function to clean up game state for a new street
    // StreetState: Resets the street state
    // ActionManager: Clears action timeline and the action state struct
    // PotManager: Calculates pots for the street and resets players' most recent bet to 9
    void cleanupStreet();

    // Street helper function to create an action object from a client object
    shared_ptr<Action> createAction(const ClientAction& clientAction, size_t initialChips);

    // Street helper function to check if all players have acted in a given round
    bool isStreetOver(int initialPlayersInhand);

    // Street helper function to update the street state after fetching the current player
    void udpateStreetStateForCurPlayer(const shared_ptr<Player>& player);

    // Round helper function to populate the hashmap in the hand evaluator class
    // playerHands maps players to their hole and community cards
    void populatePlayerHandsMap();

    // Round function to award pots when betting action is complete
    void evaluateHandsAndAwardPots();

    // Round helper function to resets the game state before a new round
    // Called at the end of each round
    // TurnManager: Resets folded players and rotates posiitions
    // ActionManager: Clear the action timeline
    // PotManager: Reset recent bets and dead money
    // HandEvaluator: Clear the playerHands map
    void setupNewRound();

    // Round helper function to award pots after betting action is complete
    // Evaluates hands and ranks players according to hand strength in HandEvaluator
    // Then, awards pots based on this player ranking
    void evaluatePots();

    // Game helper function if there are at least two players in the game
    bool verifyNumPlayers();

    // Game helper function to handle player addition and removal
    // and removal of chip verification before the start of each round
    bool verifyGamePlayers();

    // Helper function to validate player name input
    string queryPlayerName();

    // Helper function to validate player chips input
    size_t queryPlayerChips();

    // Helper function to validate chip counts of players
    void validateChipCounts();

    // Helper function to query the number of chips to add
    size_t queryChipsToAdd(size_t minChips, size_t currentChips) const;
public:
    GameController(size_t smallBlind, size_t bigBlind);
    
    // Initiates a new betting street
    void startStreet(Street newStreet);

    // Initiates a new round of poker
    void startRound();

    // Main game method
    void main();

    // STDOUT METHODS

    // Displays names and chips for players in the game
    void displayPlayersInGame() const;

    // Query client if they want to add a new player
    void queryNewPlayer();

    // Query client if they want to remove a player
    void queryRemovePlayer();
};

#endif // GAME_CONTROLLER