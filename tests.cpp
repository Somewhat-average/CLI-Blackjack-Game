#include <cassert>
#include "deck.h"
#include "hand.h"
#include "player.h"
#include "dealer.h"

void test_deck() {
    Deck deck;
    assert(deck.drawCard().value >= 2 && deck.drawCard().value <= 11);
}

void test_hand() {
    Hand hand;
    Card card1("♠", "A", 11);
    Card card2("♠", "10", 10);

    hand.addCard(card1);
    hand.addCard(card2);

    assert(hand.calculateValue() == 21);
}

void test_player() {
    Player player(100);
    player.placeBet(10);
    assert(player.balance == 90);

    player.win(10);
    assert(player.balance == 110);

    Card card1("♠", "A", 11);
    Card card2("♠", "10", 10);

    player.resetHands();
    player.addCardToHand(card1, 0);
    player.addCardToHand(card2, 0);

    assert(player.getHandValue(0) == 21);

    // Test splitHand
    Card card3("♦", "8", 8);
    Card card4("♦", "8", 8);
    Card card5("♥", "3", 3);
    Card card6("♣", "4", 4);

    player.resetHands();
    player.addCardToHand(card3, 0);
    player.addCardToHand(card4, 0);

    assert(player.getHandValue(0) == 16);
    assert(player.getNumberOfHands() == 1);

    player.splitHand(0, card5, card6);

    assert(player.getNumberOfHands() == 2);
    assert(player.getHandValue(0) == 11);
    assert(player.getHandValue(1) == 12);
}

void test_dealer() {
    Dealer dealer;
    Card card1("♠", "A", 11);
    Card card2("♠", "10", 10);

    dealer.addCardToHand(card1);
    dealer.addCardToHand(card2);

    assert(dealer.getHandValue() == 21);
}

int main() {
    test_deck();
    test_hand();
    test_player();
    test_dealer();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
