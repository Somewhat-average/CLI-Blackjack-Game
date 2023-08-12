#pragma once

#include <vector>
#include "card.h"

class Hand {
public:
    std::vector<Card> cards;

    int size() const;
    void addCard(Card card);
    int calculateValue() const;
};
