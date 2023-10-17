#pragma once

#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "pcg.h"
#include "card.h"

class Deck {
private:
    size_t currentIndex;
    std::vector<Card> deck;
    PCG engine;

public:
    Deck(int numberOfDecks);

    void shuffle();
    Card drawCard();
};
