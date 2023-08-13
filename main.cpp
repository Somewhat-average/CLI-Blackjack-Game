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

using namespace std;

int main() {
    shared_ptr<Player> player = make_shared<Player>(1000);
    BlackjackGame game(player);
    game.start(); // number of rounds, default is indefinite

    return 0;
}
