#pragma once

#include <vector>
#include <chrono>
#include <random>
#include "pcg_random.hpp"
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