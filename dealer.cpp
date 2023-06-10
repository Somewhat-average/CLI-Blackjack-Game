#include "dealer.h"

void Dealer::addCardToHand(const Card& card) {
    hand.addCard(card);
}

int Dealer::getHandValue() const {
    return hand.calculateValue();
}

void Dealer::resetHand() {
    hand.cards.clear();
}
