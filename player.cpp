#include "Player.h"

Player::Player(int initial_balance) : balance(initial_balance) {
    hands.push_back(Hand());
}

void Player::addCardToHand(const Card& card, int handIndex) {
    if (handIndex >= 0 && static_cast<size_t>(handIndex) < hands.size()) {
        hands[handIndex].cards.push_back(card);
    }
}

int Player::getHandValue(int handIndex) const {
    if (handIndex >= 0 && static_cast<size_t>(handIndex) < hands.size()) {
        return hands[handIndex].calculateValue();
    }
    return 0;
}

void Player::placeBet(int bet) {
    balance -= bet;
    bets.push_back(bet);
}

void Player::win(int amount) {
    balance += 2 * amount;
}

void Player::blackjackWin(int amount) {
    balance += amount * 2.5;
}

void Player::push(int handIndex) {
    balance += bets[handIndex];
}

int Player::getNumberOfHands() const {
    return hands.size();
}

void Player::refundBet(int bet) {
    balance += bet;
}

void Player::winBet(int bet) {
    balance += bet;
}

void Player::resetHands() {
    hands.clear();
    hands.push_back(Hand());
}

void Player::doubleDown(int handIndex, const Card &card) {
    if (handIndex >= 0 && static_cast<size_t>(handIndex) < hands.size()) {
        hands[handIndex].cards.push_back(card);
        balance -= bets[handIndex];
        bets[handIndex] *= 2;
    }
}

void Player::splitHand(int handIndex, const Card &card1, const Card &card2) {
    if (handIndex >= 0 && static_cast<size_t>(handIndex) < hands.size()) {
        if (hands[handIndex].cards.size() == 2 && hands[handIndex].cards[0].rank == hands[handIndex].cards[1].rank) {
            if (balance >= bets[handIndex]) {
                Hand newHand;

                newHand.cards.push_back(hands[handIndex].cards[1]);
                newHand.cards.push_back(card2);

                hands[handIndex].cards.pop_back();
                hands[handIndex].cards.push_back(card1);

                hands.push_back(newHand);

                bets.push_back(bets[handIndex]);
                balance -= bets[handIndex];
            } else {
                std::cerr << "Insufficient balance to split hand.\n";
            }
        } else {
            std::cerr << "Cannot split hand. Cards must be of the same rank.\n";
        }
    } else {
        std::cerr << "Invalid hand index.\n";
    }
}


