#include "blackjack.h"

BlackjackGame::BlackjackGame(std::shared_ptr<Player> player) : deck(6), player(player){}

void BlackjackGame::playRound() {
    // Place bet
    int bet = ui.promptBet(player->balance, MIN_BET, MAX_BET);
    player->placeBet(bet);

    // Deal initial cards
    player->addCardToHand(deck.drawCard(), 0);
    dealer.addCardToHand(deck.drawCard());
    player->addCardToHand(deck.drawCard(), 0);
    dealer.addCardToHand(deck.drawCard());

    // Display the initial game state
    ui.displayInitialGameState(player, dealer);

    // Check for natural blackjacks
    bool playerHasBlackjack = player->getHandValue(0) == BLACKJACK;
    bool dealerHasBlackjack = dealer.hand.calculateValue() == BLACKJACK;

    if (playerHasBlackjack || dealerHasBlackjack) {
        ui.displayBlackjackOutcome(playerHasBlackjack, dealerHasBlackjack);
        if (playerHasBlackjack && dealerHasBlackjack) {
            player->push(0);
        } else if (playerHasBlackjack) {
            player->blackjackWin(0);
        }
        
        player->resetHands();
        player->resetBets();
        dealer.resetHand();
        
        return;
    }

    // Player's turn
    bool allPlayerHandsBusted = true;

    for (int currentHand = 0; currentHand < player->getNumberOfHands(); ++currentHand) {
        while (player->getHandValue(currentHand) < BLACKJACK) {
            char action = ui.promptAction();

            if (action == 'h') {
                player->addCardToHand(deck.drawCard(), currentHand);
                ui.displayHand(player->hands[currentHand]);
            } else if (action == 's') {
                break;
            } else if (action == 'd') {
                if (player->doubleDown(currentHand, deck.drawCard())) {
                    ui.displayHand(player->hands[currentHand]);
                    if (player->getHandValue(currentHand) > BLACKJACK) {
                        // Player busted after doubling down
                        allPlayerHandsBusted = true;
                    }
                    break;
                } else {
                    ui.displayError("You cannot double down this hand");
                }

            } else if (action == 'p') {
                if (player->splitHand(currentHand, deck.drawCard(), deck.drawCard())) {
                    ui.displayGameState(player, dealer);
                } else {
                    ui.displayError("You cannot split this hand");
                }
            }
        }

        if (player->getHandValue(currentHand) <= BLACKJACK) {
            allPlayerHandsBusted = false;
        }
    }

    // Dealer's turn
    if (!allPlayerHandsBusted) {
        while (dealer.hand.calculateValue() < 17) {
            dealer.addCardToHand(deck.drawCard());
        }

        ui.displayGameState(player, dealer);

        for (int currentHand = 0; currentHand < player->getNumberOfHands(); ++currentHand) {
            if (player->getHandValue(currentHand) <= BLACKJACK) {
                int dealerHandValue = dealer.hand.calculateValue();
                if (dealerHandValue > BLACKJACK || dealerHandValue < player->getHandValue(currentHand)) {
                    player->win(currentHand);
                } else if (dealerHandValue == player->getHandValue(currentHand)) {
                    player->push(currentHand);
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
    ui.startGame();

    if (numberOfRounds <= 0) {
        // std::cout << "Starting an indefinite number of rounds." << std::endl;
        while (player->balance > MIN_BET) {
            playRound();
        }
        std::cout << "You have run out of money. Game Over!" << std::endl;
    } else {
        // std::cout << "Starting " << numberOfRounds << " rounds." << std::endl;
        for (int i = 0; i < numberOfRounds && player->balance > MIN_BET; ++i) {
            playRound();
        }
        if (player->balance < MIN_BET) {
            std::cout << "You have run out of money. Game Over!" << std::endl;
        } else {
            std::cout << "Game Over! You finished with a balance of $" << player->balance << std::endl;
        }
    }
}
