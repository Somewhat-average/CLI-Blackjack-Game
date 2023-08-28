#include <iostream>
#include <cassert>
#include <memory>
#include "card.h"
#include "deck.h"
#include "hand.h"
#include "player.h"
#include "dealer.h"
#include "ui.h"
#include "blackjack.h"

int main() {
    std::shared_ptr<Player> player = std::make_shared<Player>(500);
    BlackjackGame game(player);
    game.start(); // number of rounds, default is indefinite

    return 0;
}
