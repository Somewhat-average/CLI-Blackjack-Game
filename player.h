#pragma once

#include <vector>
#include "hand.h"

class Player {
public:
    int balance;
    std::vector<Hand> hands;
    std::vector<int> bets;

    Player(int initial_balance);

    void placeBet(int bet);
    void win(int handIndex);
    void blackjackWin(int handIndex);
    void push(int handIndex);
    void addCardToHand(const Card& card, int handIndex);
    int getHandValue(int handIndex) const;
    int getNumberOfHands() const;
    void resetHands();
    void resetBets();
    bool doubleDown(int handIndex, const Card& card);
    bool splitHand(int handIndex, const Card& card1, const Card& card2);

private:
    int calculateHandValue(const Hand &hand) const;
};
