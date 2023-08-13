#include "ui.h"
#include <iostream>
#include <string>
#include <cctype> // Include this header for the tolower function
#include <limits>

const char INVALID_ACTION = ' ';

void BlackjackUI::startGame() {
    std::cout << R"(
 ________  ___      _________  ________  ___  __          ___  ________  ________  ____ __       
|\   __  \|\  \     |\   __  \|\   ____\|\  \|\  \       |\  \|\   __  \|\   ____\|\  \|\  \
\ \  \|\ /\ \  \    \ \  \|\  \ \  \___|\ \  \/  /|_     \ \  \ \  \|\  \ \  \___|\ \  \/  /|_   
 \ \   __  \ \  \    \ \   __  \ \  \    \ \   ___  \  __ \ \  \ \   __  \ \  \    \ \   ___  \
  \ \  \|\  \ \  \____\ \  \ \  \ \  \____\ \  \\ \  \|\  \\_\  \ \  \ \  \ \  \____\ \  \\ \  \
   \ \_______\ \_______\ \__\ \__\ \_______\ \__\\ \__\ \________\ \__\ \__\ \_______\ \__\\ \__\
    \|_______|\|_______|\|__|\|__|\|_______|\|__| \|__|\|________|\|__|\|__|\|_______|\|__| \|__|
                                                                                                                                                                                           
)";
    std::cout << "Available actions: (h: Hit, s: Stand, d: Double, p: Split)\n\n";
}

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
        std::cout << "\nEnter action: ";
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
                    displayInvalidActionError();
                    break;
            }
        } else {
            displayInvalidActionError();
        }
    }

    // This line should never be reached, but it's needed to prevent a compiler warning.
    return INVALID_ACTION;
}

void BlackjackUI::displayInvalidActionError() {
    std::cerr << "Invalid action. Please enter 'h', 's', 'd', or 'p'.\n";
}

void BlackjackUI::displayError(const std::string& errorMessage) {
    std::cout << "Error: " << errorMessage << std::endl;
}

void BlackjackUI::displayBlackjackOutcome(bool playerHasBlackjack, bool dealerHasBlackjack) {
    if (playerHasBlackjack && dealerHasBlackjack) {
        std::cout << "Both player and dealer have blackjack. It's a push!\n\n";
    } else if (playerHasBlackjack) {
        std::cout << "Player has blackjack!\n\n";
    } else if (dealerHasBlackjack) {
        std::cout << "Dealer has blackjack!\n\n";
    }
}

void BlackjackUI::displayInitialGameState(const std::shared_ptr<Player>& playerPtr, const Dealer& dealer) {
    Player& player = *playerPtr.get();
    std::cout << "\nDealer's upcard: ";
    displayCard(dealer.hand.cards[0]);
    std::cout << '\n';

    // std::cout << "Player's hand(s):\n";
    for (int i = 0; i < player.getNumberOfHands(); ++i) {
        std::cout << "Hand " << i + 1 << ": ";
        displayHand(player.hands[i]);
    }
}

void BlackjackUI::displayGameState(const std::shared_ptr<Player>& playerPtr, const Dealer& dealer) {
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
            std::cout << "Player's hand " << i + 1 << " busted.\n\n";
        } else if (dealerHandValue > 21 || dealerHandValue < playerHandValue) {
            std::cout << "Player's hand " << i + 1 << " wins!\n\n";
        } else if (dealerHandValue == playerHandValue) {
            std::cout << "Player's hand " << i + 1 << " pushes (draw).\n\n";
        } else {
            std::cout << "Player's hand " << i + 1 << " loses.\n\n";
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
