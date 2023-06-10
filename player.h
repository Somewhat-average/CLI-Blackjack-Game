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
    void win(int amount);
    void blackjackWin(int amount);
    void push(int handIndex);
    void addCardToHand(const Card& card, int handIndex);
    int getHandValue(int handIndex) const;
    int getNumberOfHands() const;
    void refundBet(int betAmount);
    void winBet(int betAmount);
    void resetHands();
    void doubleDown(int handIndex, const Card& card);
    void splitHand(int handIndex, const Card& card1, const Card& card2);

private:
    int calculateHandValue(const Hand &hand) const;
};