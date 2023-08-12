#pragma once

#include "hand.h"
#include "card.h"

class Dealer {
public:
    Hand hand;

    void addCardToHand(const Card& card);
    int getHandValue() const;
    void resetHand();
};
