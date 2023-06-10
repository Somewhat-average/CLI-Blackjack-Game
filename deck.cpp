#include "deck.h"

Deck::Deck() {
	std::vector<std::string> suits = {"♥", "♦", "♣", "♠"};
	std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
	for (const auto& rank : ranks) {
		for (const auto& suit : suits) {
	        int value;
	        if (rank == "J" || rank == "Q" || rank == "K") {
	            value = 10;
	        } else if (rank == "A") {
	            value = 11;
	        } else {
	            value = stoi(rank);
	        }
	        deck.push_back(Card(suit, rank, value));
	    }
	}
	//shuffleDeck();
}

void Deck::shuffleDeck() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
}

Card Deck::drawCard() {
    Card card = deck.back();
    deck.pop_back();
    return card;
}