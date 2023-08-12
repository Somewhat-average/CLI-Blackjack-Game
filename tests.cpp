#include <cassert>
#include "deck.h"
#include "hand.h"
#include "player.h"
#include "dealer.h"

const bool DEBUG = false;

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

void test_double_down() {
    Player player(100);
    player.placeBet(10);
    assert(player.balance == 90);

    Card card1("♠", "5", 5);
    Card card2("♠", "6", 6);
    Card card3("♠", "7", 7);

    player.resetHands();
    player.addCardToHand(card1, 0);
    player.addCardToHand(card2, 0);

    assert(player.getHandValue(0) == 11);

    player.doubleDown(0, card3);

    assert(player.getHandValue(0) == 18);
    assert(player.balance == 80); // Check if the bet was doubled
}

void test_double_down_balance() {
    if (DEBUG)
        std::cout << "\ntest_double_down_balance:\n";
    Player player(1200);
    Dealer dealer;

    // Initial bet
    player.placeBet(200);
    if (DEBUG)
        std::cout << "After initial bet, balance: " << player.balance << std::endl;

    // Cards for the player and dealer
    Card playerCard1("♠", "Q", 10);
    Card playerCard2("♠", "Q", 10);
    Card doubleDownCard("♦", "J", 10);  // This card ensures the player busts
    Card dealerCard1("♣", "Q", 10);
    Card dealerCard2("♣", "9", 9);

    // Player's hand
    player.addCardToHand(playerCard1, 0);
    player.addCardToHand(playerCard2, 0);

    // Player doubles down
    player.doubleDown(0, doubleDownCard);
    if (DEBUG)
        std::cout << "After double down, balance: " << player.balance << std::endl;

    // Dealer's hand
    dealer.addCardToHand(dealerCard1);
    dealer.addCardToHand(dealerCard2);

    if (player.getHandValue(0) > 21) {
        // Player busted, no action needed as the bet is already lost
    } else if (dealer.getHandValue() > 21 || dealer.getHandValue() < player.getHandValue(0)) {
        player.win(0); // Add winnings for the double down
    }

    if (DEBUG)
        std::cout << "After comparing with dealer, balance: " << player.balance << std::endl;
    assert(player.balance == 800);  // 1200 - 200 - 200 (due to double down)
}

void test_push_after_double_down() {
    if (DEBUG)
        std::cout << "\ntest_push_after_double_down:\n";
    Player player(1000);
    Dealer dealer;

    // Initial bet
    player.placeBet(100);
    if (DEBUG)
        std::cout << "After initial bet, balance: " << player.balance << std::endl;
    assert(player.balance == 900);

    // Cards for the player and dealer
    Card playerCard1("♠", "5", 5);
    Card playerCard2("♠", "6", 6);
    Card doubleDownCard("♣", "6", 6);  // This card ensures the player's total is 17
    Card dealerCard1("♠", "K", 10);
    Card dealerCard2("♠", "7", 7);  // Dealer's hand is 17, which pushes with the player's hand

    // Player's hand
    player.addCardToHand(playerCard1, 0);
    player.addCardToHand(playerCard2, 0);

    // Player doubles down
    player.doubleDown(0, doubleDownCard);
    if (DEBUG)
        std::cout << "After double down, balance: " << player.balance << std::endl;

    // Dealer's hand
    dealer.addCardToHand(dealerCard1);
    dealer.addCardToHand(dealerCard2);

    // Check if the player's hand pushes with the dealer's hand
    if (dealer.getHandValue() == player.getHandValue(0)) {
        player.push(0); // Refund the bet for the hand
    }

    if (DEBUG)
        std::cout << "After comparing with dealer, balance: " << player.balance << std::endl;
    assert(player.balance == 1000);  // 1000 - 100 - 100 (due to double down) + 200 (push refund)
}

void test_split() {
    Player player(100);
    player.placeBet(10);
    assert(player.balance == 90);

    Card card1("♠", "8", 8);
    Card card2("♠", "8", 8);
    Card card3("♦", "5", 5);
    Card card4("♦", "6", 6);

    player.resetHands();
    player.addCardToHand(card1, 0);
    player.addCardToHand(card2, 0);

    assert(player.getHandValue(0) == 16);
    assert(player.getNumberOfHands() == 1);

    player.splitHand(0, card3, card4);

    assert(player.getNumberOfHands() == 2);
    assert(player.getHandValue(0) == 13); // 8 + 5
    assert(player.getHandValue(1) == 14); // 8 + 6
    assert(player.balance == 80); // Check if the bet was placed for the second hand
}

void test_double_down_after_split() {
    Player player(100);
    player.placeBet(10);
    assert(player.balance == 90);

    Card card1("♠", "8", 8);
    Card card2("♠", "8", 8);
    Card card3("♦", "5", 5);
    Card card4("♦", "6", 6);
    Card card5("♣", "7", 7);

    player.resetHands();
    player.addCardToHand(card1, 0);
    player.addCardToHand(card2, 0);

    player.splitHand(0, card3, card4);

    assert(player.getNumberOfHands() == 2);

    player.doubleDown(0, card5); // Double down on the first split hand

    assert(player.getHandValue(0) == 20); // 8 + 5 + 7
    assert(player.balance == 70); // Check if the bet was doubled for the first split hand
}

void test_push() {
    if (DEBUG)
        std::cout << "\ntest_push:\n";
    Player player(1000);
    Dealer dealer;

    // Initial bet
    player.placeBet(1000);
    if (DEBUG)
        std::cout << "After initial bet, balance: " << player.balance << std::endl;
    assert(player.balance == 0);

    // Cards for the player and dealer
    Card playerCard1("♠", "K", 10);
    Card playerCard2("♠", "J", 10);  // Player's hand is 20
    Card dealerCard1("♣", "4", 4);
    Card dealerCard2("♣", "5", 5);
    Card dealerCard3("♣", "6", 6);
    Card dealerCard4("♣", "5", 5);  // Dealer's hand is 20, which pushes with the player's hand

    // Player's hand
    player.addCardToHand(playerCard1, 0);
    player.addCardToHand(playerCard2, 0);

    // Dealer's hand
    dealer.addCardToHand(dealerCard1);
    dealer.addCardToHand(dealerCard2);
    dealer.addCardToHand(dealerCard3);
    dealer.addCardToHand(dealerCard4);

    // Check if the player's hand pushes with the dealer's hand
    if (dealer.getHandValue() == player.getHandValue(0)) {
        player.push(0); // Refund the bet for the hand
    }

    if (DEBUG)
        std::cout << "After comparing with dealer, balance: " << player.balance << std::endl;
    assert(player.balance == 1000);  // 1000 - 1000 (bet) + 1000 (push refund)
}

void test_player() {
    Player player(100);
    player.placeBet(10);
    assert(player.balance == 90);

    player.win(0);
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

void test_balance_after_actions() {
    if (DEBUG)
        std::cout << "\ntest_balance_after_actions:\n";
    Player player(100);
    Dealer dealer;

    // Initial bet
    player.placeBet(10);
    if (DEBUG)
        std::cout << "After initial bet, balance: " << player.balance << std::endl;
    assert(player.balance == 90);

    // Cards for the player and dealer
    Card playerCard1("♠", "8", 8);
    Card playerCard2("♠", "8", 8);
    Card splitCard1("♦", "5", 5);
    Card splitCard2("♦", "6", 10);
    Card doubleDownCard("♣", "2", 2);  // This card ensures the first hand loses
    Card dealerCard1("♠", "10", 10);
    Card dealerCard2("♠", "7", 7);  // Dealer's hand is 17, which beats the first hand but loses to the second

    // Player splits
    player.resetHands();
    player.addCardToHand(playerCard1, 0);
    player.addCardToHand(playerCard2, 0);
    player.splitHand(0, splitCard1, splitCard2);
    if (DEBUG)
        std::cout << "After split, balance: " << player.balance << std::endl;

    // Player doubles down on the first split hand
    player.doubleDown(0, doubleDownCard);
    if (DEBUG)
        std::cout << "After double down, balance: " << player.balance << std::endl;

    // Dealer's hand
    dealer.addCardToHand(dealerCard1);
    dealer.addCardToHand(dealerCard2);

    // Check if the player's hand wins or loses against the dealer's hand
    if (dealer.getHandValue() > 21 || dealer.getHandValue() < player.getHandValue(0)) {
        player.win(0); // Add winnings for the double down
    }
    if (dealer.getHandValue() > 21 || dealer.getHandValue() < player.getHandValue(1)) {
        player.win(1); // Add winnings for the second hand
    }

    if (DEBUG)
        std::cout << "After comparing with dealer, balance: " << player.balance << std::endl;
    assert(player.balance == 90);
}

int main() {
    // basic tests
    test_deck();
    test_hand();
    test_player();
    test_dealer();
    test_push();
    test_double_down();
    test_split();

    // advanced tests
    test_double_down_after_split();
    test_balance_after_actions();
    test_double_down_balance();
    test_push_after_double_down();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
