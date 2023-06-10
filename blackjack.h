#pragma once

#include <memory>
#include "deck.h"
#include "dealer.h"
#include "ui.h"

class BlackjackGame {
private:
    Deck deck;
    std::shared_ptr<Player> player;
    Dealer dealer;
    BlackjackUI ui;

    void playRound();

public:
    BlackjackGame(std::shared_ptr<Player> player);

    void start(int numberOfRounds = -1);
};