#include "blackjack.h"

BlackjackGame::BlackjackGame(std::shared_ptr<Player> player) : player(player) {}

void BlackjackGame::playRound() {
    // Place bet
    int bet = ui.promptBet(player->balance);
    std::cout << "bet entered: " << bet << std::endl;
    player->placeBet(bet);

    // Deal initial cards
    player->addCardToHand(deck.drawCard(), 0);
    dealer.addCardToHand(deck.drawCard());
    player->addCardToHand(deck.drawCard(), 0);
    dealer.addCardToHand(deck.drawCard());

    // Display the initial game state
    ui.displayInitialGameState(player, dealer);

    // Check for natural blackjacks
    bool playerHasBlackjack = player->getHandValue(0) == 21;
    bool dealerHasBlackjack = dealer.hand.calculateValue() == 21;

    if (playerHasBlackjack || dealerHasBlackjack) {
        ui.displayBlackjackOutcome(playerHasBlackjack, dealerHasBlackjack);
        if (playerHasBlackjack && dealerHasBlackjack) {
            player->refundBet(bet);
        } else if (playerHasBlackjack) {
            player->blackjackWin(bet);
        }
        return;
    }

    // Player's turn
    bool allPlayerHandsBusted = true;

    for (int currentHand = 0; currentHand < player->getNumberOfHands(); ++currentHand) {
        while (player->getHandValue(currentHand) < 21) {
            char action = ui.promptAction();

            if (action == 'h') {
                player->addCardToHand(deck.drawCard(), currentHand);
                ui.displayHand(player->hands[currentHand]);
            } else if (action == 's') {
                break;
            } else if (action == 'd') {
                player->doubleDown(currentHand, deck.drawCard());
                ui.displayHand(player->hands[currentHand]);
                if (player->getHandValue(currentHand) > 21) {
                    // Player busted after doubling down
                    allPlayerHandsBusted = true;
                }
                break;
            } else if (action == 'p') {
                player->splitHand(currentHand, deck.drawCard(), deck.drawCard());
            }
        }

        if (player->getHandValue(currentHand) <= 21) {
            allPlayerHandsBusted = false;
        }
    }

    // Dealer's turn
    if (!allPlayerHandsBusted) {
        while (dealer.hand.calculateValue() < 17) {
            dealer.addCardToHand(deck.drawCard());
        }

        ui.displayGameState(player, dealer, -1);

        for (int i = 0; i < player->getNumberOfHands(); ++i) {
            int currentBet = player->bets[i];  // Get the actual bet for the current hand
            if (player->getHandValue(i) <= 21) {
                int dealerHandValue = dealer.hand.calculateValue();
                if (dealerHandValue > 21 || dealerHandValue < player->getHandValue(i)) {
                    player->win(currentBet);  // Use the actual bet instead of the original bet
                } else if (dealerHandValue == player->getHandValue(i)) {
                    player->push(currentBet);  // Use the actual bet instead of the original bet
                }
            }
        }
    }

    ui.displayRoundResult(player, dealer);

    player->resetHands();
    player->resetBets();
    dealer.resetHand();
}

void BlackjackGame::start(int numberOfRounds) {
    if (numberOfRounds <= 0) {
        // std::cout << "Starting an indefinite number of rounds." << std::endl;
        while (player->balance > 0) {
            playRound();
        }
        std::cout << "You have run out of money. Game Over!" << std::endl;
    } else {
        // std::cout << "Starting " << numberOfRounds << " rounds." << std::endl;
        for (int i = 0; i < numberOfRounds && player->balance > 0; ++i) {
            playRound();
        }
        if (player->balance == 0) {
            std::cout << "You have run out of money. Game Over!" << std::endl;
        } else {
            std::cout << "Game Over! You finished with a balance of $" << player->balance << std::endl;
        }
    }
}
