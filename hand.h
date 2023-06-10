#pragma once

#include <vector>
#include "card.h"

class Hand {
public:
    std::vector<Card> cards;

    void addCard(Card card);
    int calculateValue() const;
};