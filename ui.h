#pragma once

#include "dealer.h"
#include "player.h"
#include <memory>

class BlackjackUI {
public:
    int promptBet(int balance, const int MIN_BET, const int MAX_BET);
    char promptAction();
    void startGame();
    void displayInvalidActionError();
    void displayError(const std::string& errorMessage);
    void displayBlackjackOutcome(bool playerHasBlackjack, bool dealerHasBlackjack);
    void displayInitialGameState(const std::shared_ptr<Player>& playerPtr, const Dealer& dealer);
    void displayGameState(const std::shared_ptr<Player>& playerPtr, const Dealer& dealer);
    void displayRoundResult(const std::shared_ptr<Player>& playerPtr, const Dealer& dealer);
    void displayHand(const Hand& hand);
    void displayCard(const Card& card);
};
