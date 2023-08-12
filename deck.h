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
    Deck(int numberOfDecks);

    void shuffleDeck();
    Card drawCard();
};
