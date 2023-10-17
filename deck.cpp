#include "deck.h"

Deck::Deck(int numberOfDecks) 
    : engine(std::chrono::high_resolution_clock::now().time_since_epoch().count(), static_cast<uint64_t>(std::random_device()())) // Use the PCG class with seed and sequence
{
	const std::vector<std::string> suits = {"♥", "♦", "♣", "♠"};
	const std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

	for (int deckNum=0; deckNum < numberOfDecks; ++deckNum) {
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
	}
	currentIndex = 0;
    shuffle();
}


void Deck::shuffle() {
    std::shuffle(deck.begin(), deck.end(), engine);
}


Card Deck::drawCard() {
    if (currentIndex >= deck.size()) {
        // Reshuffle the deck if currentIndex exceeds the number of cards in the deck
        shuffle();
        currentIndex = 0;  // Reset the currentIndex to 0
    }

    Card card = deck[currentIndex];
    currentIndex++;
    return card;
}
