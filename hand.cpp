#include "hand.h"

void Hand::addCard(Card card) {
    cards.push_back(card);
}

int Hand::calculateValue() const {
    int value = 0;
    int aces = 0;
    for (const auto& card : cards) {
        value += card.value;
        if (card.rank == "A") {
            aces++;
        }
    }
    while (value > 21 && aces > 0) {
        value -= 10;
        aces--;
    }
    return value;
}