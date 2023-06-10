#pragma once

#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "card.h"

class Deck {
private:
    std::vector<Card> deck;

public:
    Deck();

    void shuffleDeck();
    Card drawCard();
};