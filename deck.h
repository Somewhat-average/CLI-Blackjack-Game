#pragma once

#include <vector>
#include <chrono>
#include <random>
#include "pcg.h"
#include <algorithm>
#include "card.h"


class Deck {
private:
    size_t currentIndex;
    std::vector<Card> deck;

public:
    Deck();

    void shuffleDeck();
    Card drawCard();
};