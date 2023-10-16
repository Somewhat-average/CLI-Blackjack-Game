#include "deck.h"

Deck::Deck(int numberOfDecks) {
    // Create a PCG engine and seed it with a combination of random device and chrono
    std::random_device rd;
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    PCG engine(seed, rd());  // Use the PCG class with seed and sequence
    
	std::vector<std::string> suits = {"♥", "♦", "♣", "♠"};
	std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

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
	shuffleDeck();
}


void Deck::shuffleDeck() {
    // Shuffle the elements in the cards_ container using the random engine
    std::shuffle(deck.begin(), deck.end(), engine);
}


Card Deck::drawCard() {
    if (currentIndex >= deck.size()) {
        // Reshuffle the deck if currentIndex exceeds the number of cards in the deck
        shuffleDeck();  // Reshuffle the deck
        currentIndex = 0;  // Reset the currentIndex to 0
    }

    Card card = deck[currentIndex];
    currentIndex++;
    return card;
}
