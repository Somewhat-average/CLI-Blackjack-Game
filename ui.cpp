#include "ui.h"
#include <iostream>
#include <string>
#include <cctype> // Include this header for the tolower function
#include <limits>

int BlackjackUI::promptBet(int balance) {
    const int MIN_BET = 1;
    int bet = 0;
    bool validInput = false;

    do {
        std::cout << "Your current balance is: $" << balance << '\n';
        std::cout << "Enter your bet: ";

        if (!(std::cin >> bet)) {
            std::cerr << "Invalid input. Please enter a valid numeric bet.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (bet < MIN_BET || bet > balance) {
            std::cerr << "Invalid bet. Please enter a bet between $" << MIN_BET << " and your current balance.\n";
        } else {
            validInput = true;
        }
    } while (!validInput);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return bet;
}

char BlackjackUI::promptAction() {
    std::string input;
    bool validInput = false;

    while (!validInput) {
        std::cout << "Enter action (h: Hit, s: Stand, d: Double, p: Split): ";
        std::getline(std::cin, input);

        if (!input.empty()) {
            char action = std::tolower(input[0]);
            switch (action) {
                case 'h':
                case 's':
                case 'd':
                case 'p':
                    validInput = true;
                    return action;
                default:
                    std::cerr << "Invalid input. Please try again.\n";
                    break;
            }
        } else {
            std::cerr << "Invalid input. Please try again.\n";
        }
    }

    // This line should never be reached, but it's needed to prevent a compiler warning.
    return ' ';
}

void BlackjackUI::displayBlackjackOutcome(bool playerHasBlackjack, bool dealerHasBlackjack) {
    if (playerHasBlackjack && dealerHasBlackjack) {
        std::cout << "Both player and dealer have blackjack. It's a push!\n";
    } else if (playerHasBlackjack) {
        std::cout << "Player has blackjack!\n";
    } else if (dealerHasBlackjack) {
        std::cout << "Dealer has blackjack!\n";
    }
}

void BlackjackUI::displayInitialGameState(const std::shared_ptr<Player>& playerPtr, const Dealer& dealer) {
    Player& player = *playerPtr.get();
    std::cout << "Dealer's upcard: ";
    displayCard(dealer.hand.cards[0]);
    std::cout << '\n';

    // std::cout << "Player's hand(s):\n";
    for (int i = 0; i < player.getNumberOfHands(); ++i) {
        std::cout << "Hand " << i + 1 << ": ";
        displayHand(player.hands[i]);
    }
}

void BlackjackUI::displayGameState(const std::shared_ptr<Player>& playerPtr, const Dealer& dealer, int currentHand) {
    Player& player = *playerPtr.get();
    std::cout << "Dealer's hand: \n";
    displayHand(dealer.hand);
    std::cout << "\n";

    std::cout << "Player's hand(s):\n";
    for (int i = 0; i < player.getNumberOfHands(); ++i) {
        std::cout << "Hand " << i + 1 << ": \n";
        displayHand(player.hands[i]);
        std::cout << '\n';
    }
}

void BlackjackUI::displayRoundResult(const std::shared_ptr<Player>& playerPtr, const Dealer& dealer) {
    Player& player = *playerPtr.get();
    int dealerHandValue = dealer.hand.calculateValue();

    std::cout << "Round result:\n";

    for (int i = 0; i < player.getNumberOfHands(); ++i) {
        int playerHandValue = player.getHandValue(i);

        if (playerHandValue > 21) {
            std::cout << "Player's hand " << i + 1 << " busted.\n";
        } else if (dealerHandValue > 21 || dealerHandValue < playerHandValue) {
            std::cout << "Player's hand " << i + 1 << " wins!\n";
        } else if (dealerHandValue == playerHandValue) {
            std::cout << "Player's hand " << i + 1 << " pushes (draw).\n";
        } else {
            std::cout << "Player's hand " << i + 1 << " loses.\n";
        }
    }
}

void BlackjackUI::displayCard(const Card& card) {
    std::cout << card.rank << card.suit << ' ';
}

void BlackjackUI::displayHand(const Hand& hand) {
    // std::cout << "Hand: ";
    for (const auto& card : hand.cards) {
        displayCard(card);
    }
    std::cout << "\nTotal: " << hand.calculateValue() << '\n';
}
