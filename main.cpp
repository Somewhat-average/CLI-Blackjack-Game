/* 
TODO:
 -add override functions to the bot class

IDEAS:
 -remove suits from the game, not useful information, only makes the output look worse, keep for ncurses, change to symbols tho
 -add graphical representations of hands with ascii cards using ncurses library

BUGS:

*/ 
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <random>
#include "pcg_random.hpp"


class Card {
public:
    enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES};
    enum Rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};

private:
    Suit suit_;
    Rank rank_;

public:
    Card(Suit suit, Rank rank) {
        suit_ = suit;
        rank_ = rank;
    }

    friend std::ostream& operator<<(std::ostream& os, const Card& card) {
    return os << card.get_rank() << " of " << card.get_suit();
    }

    int get_value() const {
        if (rank_ <= 10) {
            return rank_;
        } else {
            return 10;
        }
    }

    std::string get_suit() const {
    static const std::string suits[] = {"Clubs", "Diamonds", "Hearts", "Spades"};
    return suits[suit_];
    }

    std::string get_rank() const {
        static const std::string ranks[] = {"Unknown Rank", "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
        return ranks[rank_];
    }

    void show() const { std::cout << get_rank() << " of " << get_suit() << '\n'; }
};


class Deck {
private:
    std::vector<Card> cards_;
    int index_ = 0;

public:
    // fill the dealer's shoe
    Deck(int num_decks) {
        for (int i = 0; i < num_decks; i++) {
            for (int rank = Card::ACE; rank <= Card::KING; rank++) {
                for (int suit = Card::CLUBS; suit <= Card::SPADES; suit++) {
                    cards_.emplace_back(Card(static_cast<Card::Suit>(suit), static_cast<Card::Rank>(rank)));
                }
            }
        }
        shuffle();
    }

    void shuffle() {
        // Create a PCG engine and seed it with the current time & random device
        std::random_device rd;
        std::seed_seq seed{ rd(), static_cast<unsigned int>(time(nullptr)) };
        pcg32 engine(seed);
        
        // Shuffle the elements in the cards_ container using the random engine
        std::shuffle(cards_.begin(), cards_.end(), engine);
        
        // Reset the index to 0, since the deck is now freshly shuffled
        index_ = 0;
    }

    Card deal() {
        if (index_ == cards_.size()) {
            // The shoe is empty, so shuffle the cards and reset the index
            shuffle();
        }
        Card card = cards_[index_];
        index_++;
        return card;
    }
};


class Player {
protected: 
    // player status
    std::vector<Card> hand_;
    int balance_;
    bool is_active_;
    bool lost_;

    // splitting variables
    std::vector<Card> split_hand_;
    int split_wager_;
    bool lost_split_;
    bool lost_second_hand_;
    bool has_split_;
    bool dd_on_split_;
    bool split_aces_;

    // player betting limits
    const int LOWER_LIMIT = 2;
    const int UPPER_LIMIT = 500;

public:
    Player(int initial_balance = 500) : balance_(initial_balance), is_active_(true), lost_(false), has_split_(false), dd_on_split_(false), split_aces_(false), lost_second_hand_(false), lost_split_(false), split_wager_(0) {}

    bool is_active() const { return is_active_; }

    bool is_lost() const { return lost_; }

    bool has_split() const { return has_split_; }

    void lose_second_hand() { lost_second_hand_ = true; }

    void lose(int wager) {
        balance_ -= wager;
        is_active_ = false;
        lost_ = true;
    }

    void hit(Card card) { hand_.push_back(card); }

    virtual void double_down(int &wager, Deck &deck) {
        if (hand_.size() > 2) {
            std::cout << "Too many cards\n";
            return;
        }
        bool enough_money = true;
        if (dd_on_split_ && balance_ < 4*wager){
            enough_money = false;
        }
        if (has_split_ && balance_ < 3*wager) {
            enough_money = false;
        }
        if (balance_ < 2*wager) {
            enough_money = false;
        }

        if (!enough_money) {
            std::cout << "Not enough money\n";
            return;
        }

        wager *= 2;
        hit(deck.deal());
        get_last_card().show();
        if (total() > 21) {
            std::cout << "Bust\n\n";
            lose(wager);
            if (has_split_){
                lost_ = false;
                lost_second_hand_ = true;
            }
        }
        is_active_ = false;
    }

    virtual void split(int wager, Deck &deck) {
        if (balance_ < 2*wager) {
            std::cout << "Not enough money\n";
            return;
        }
        if (hand_.size() != 2 || hand_[0].get_rank() != hand_[1].get_rank()){
            std::cout << "Cannot split hand\n";
            return;
        }
        if (has_split_) {
            std::cout << "Cannot split again\n";
            return;
        }

        has_split_ = true;
        split_wager_ = wager;
        bool active_split = true;
        split_hand_.push_back(hand_[1]);
        hand_.pop_back();

        std::cout << "\nHand 1:\n" << split_hand_[0] << '\n';
        while (active_split) {
            
            std::cout << "Do you want to hit, stand, or double down(h/s/d)? ";
            std::string input;
            std::getline(std::cin, input);

            if (input == "h") { // hit
                split_hand_.push_back(deck.deal());
                std::cout << split_hand_.back() << '\n';
                if (split_total() > 21) {
                    std::cout << "Bust\n\n";
                    active_split = false;
                    lost_split_ = true;
                }
                if (split_hand_[0].get_rank() == "Ace"){
                  split_aces_ = true;
                  active_split = false;
                }
            } else if (input == "s") { // stand
                active_split = false;
            } else if (input == "d") { // double down
                if (balance_ >= 3*split_wager_) { // includes initial cost to split
                    if (split_hand_.size() <= 2) {
                        dd_on_split_ = true;
                        split_wager_ *= 2;
                        hit(deck.deal());
                        get_last_card().show();
                        if (split_total() > 21) {
                            std::cout << "Bust\n\n";
                            // lose money in handle_split method
                            lost_split_ = true;
                        }
                        active_split = false;                    
                    } else {
                        std::cout << "Too many cards\n";
                    }
                } else {
                    std::cout << "Not enough money\n";
                }
            } else {
                std::cout << "Invalid option, choose 'h', 's', or 'd'\n";
            }    
        }
        std::cout << "\nHand 2:\n";
        std::cout << hand_[0] << '\n';
    }

    Card get_last_card() const { return hand_.back(); }

    int get_balance() const { return balance_; }

    int get_split_wager() const { return split_wager_; }

    bool is_lost_split() const { return lost_split_; }

    bool dd_on_split() const { return dd_on_split_; }

    bool is_lost_second_hand() const { return lost_second_hand_; }

    bool is_split_aces() const { return split_aces_; }

    int get_lower_limit() const { return LOWER_LIMIT; }

    int total(const std::vector<Card> &hand) const {
        int sum = 0;
        int num_aces = 0;
        for (Card c : hand) {
            int value = c.get_value();
            if (value == Card::ACE) {
                num_aces++;
            }
            sum += value;
        }
        // if player has aces and sum with ace is less than 21, add 10 (count ace as 11)
        while (num_aces > 0 && sum <= 11) {
            sum += 10;
            num_aces--;
        }
        return sum;
    }

    int total() const { // default option
        return total(hand_);
    }

    int split_total() const {
        return total(split_hand_);
    }

    virtual void turn(int &bet, Deck &deck) {
        while (is_active()) {
            std::cout << "Do you want to hit, stand, split, or double down(h/s/sp/d)? ";
            std::string input;
            std::getline(std::cin, input);

            if (input == "h") { // hit
                hit(deck.deal());
                get_last_card().show();
                if (total() > 21) {
                    std::cout << "Bust\n\n";
                    if (has_split_) { // has split
                        end_turn();
                        lose_second_hand();
                    } else {
                        lose(bet);
                    }  
                }
                if (split_aces_) {
                  end_turn();
                }
            } else if (input == "s") { // stand
                end_turn();
            } else if (input == "d") { // double down
                if (dd_on_split_ && balance_ >= 4*bet) {
                    double_down(bet, deck);
                } else if (has_split_ && balance_ >= 3*bet) {
                    double_down(bet, deck);
                } else {
                    double_down(bet, deck);
                }
                
            } else if (input == "sp") {
                split(bet, deck);
            } else {
                std::cout << "Invalid option, choose 'h', 's', or 'd'\n";
            }    
        }
    }

    virtual int bet() const {
        int wager = 0;
        
        do { 
            while (!wager) {
              try {
                std::cout << "Enter bet amount: ";
                std::string input;
                std::getline(std::cin, input);
                wager = std::stoi(input);
              } catch (...) {
                std::cout << "Not a valid number\n";
              }
            }
            if (wager < LOWER_LIMIT) {
                std::cout << "The minimum bet is $" << LOWER_LIMIT << ".\n";
            } else if (wager > balance_) {
                std::cout << "Not enough money.\n";
            } else if (wager > UPPER_LIMIT) {
                std::cout << "The table limit is $" << UPPER_LIMIT << ".\n";
            }
        } while (wager < LOWER_LIMIT || wager > balance_ || wager > UPPER_LIMIT);
        return wager;
    }

    void win(const int wager) { balance_ += wager; }

    void end_turn() { is_active_ = false; }

    void reset() {
        hand_.clear();
        is_active_ = true;
        lost_ = false;
        
        split_hand_.clear();
        split_wager_ = 0;
        lost_split_ = false;
        has_split_ = false;
        dd_on_split_ = false;
    }
};


class Bot : public Player {
public:
    Bot(int initial_balance) : Player(initial_balance) {}

    virtual int bet() const override { // always bet $100
        const int BET = 100;
        std::cout << "Enter bet amount: $" << BET << '\n';
        return BET;
    }

    // must be in this order
    // virtual void double_down(int &wager, Deck &deck) override {}

    // virtual void split(int wager, Deck &deck) override {}
   
    // virtual void turn(int &bet, Deck &deck) override {}
};


class Dealer {
private:
    std::vector<Card> hand_;

public:
    void hit(Card card) { hand_.push_back(card); }

    Card get_last_card() const { return hand_.back(); }

    Card get_first_card() const { return hand_[0]; }

    void reset() { hand_.clear(); }

    int total() const {
        int sum = 0;
        for (Card c : hand_){
            sum += c.get_value();
        }
        return sum;
    }
};


class BlackjackGame {
private:
    Deck deck_;
    Player &player_;
    Dealer dealer_;

public:
    BlackjackGame(Player &player, int num_decks = 6) : deck_(num_decks), player_(player) {}

    void handle_split(int bet, int dealer_total) {
        bool won_hand_1 = dealer_total < player_.split_total();
        if (player_.is_lost_split()) { won_hand_1 = false; }
        bool push_hand_1 = dealer_total == player_.split_total();
        bool won_hand_2 = dealer_total < player_.total();
        if (player_.is_lost_second_hand()) { won_hand_2 = false; }
        bool push_hand_2 = dealer_total == player_.total();
        int bet_2 = player_.get_split_wager();

        if (won_hand_1 && won_hand_2) {
            std::cout << "You win both hands\n\n";
            player_.win(bet);
            player_.win(bet_2);
        } else if (won_hand_1 && push_hand_2) {
            std::cout << "You win hand 1\n\n";
            player_.win(bet_2);
        } else if (won_hand_2 && push_hand_1) {
            std::cout << "You win hand 2\n\n";
            player_.win(bet);
        } else if (won_hand_1 && !won_hand_2) {
            std::cout << "You win hand 1 and lose hand 2\n\n";
            player_.win(bet_2);
            player_.lose(bet);
        } else if (won_hand_2 && !won_hand_1) {
            std::cout << "You lose hand 1 and win hand 2\n\n";
            player_.win(bet);
            player_.lose(bet_2);
        } else if (!won_hand_1 && !won_hand_2) {
            std::cout << "Dealer wins, you lose both hands\n\n";
            player_.lose(bet);
            player_.lose(bet_2);
        } else {
            std::cout << "Push\n\n";
        }
    }
    
    void dealer_turn(int bet) {
        std::cout << "\nDealer shows: " << dealer_.get_last_card() << '\n';
        int total = dealer_.total();
        
        while (total < 17) {
            dealer_.hit(deck_.deal());
            std::cout << "Dealer shows: " << dealer_.get_last_card() << '\n';
            total = dealer_.total();
        }

        if (total > 21) {
            std::cout << "Dealer bust, you win\n\n";
            player_.win(bet);
            if (player_.get_split_wager() && !player_.is_lost_split()) {
                player_.win(player_.get_split_wager());
            }
        } else if (total >= 17) { // Dealer will hit on a soft 17
            std::cout << "Dealer stands\n";
            
            if (player_.has_split()) {
                handle_split(bet, total);
            } else {
                if (total < player_.total()) {
                    std::cout << "You win\n\n";
                    player_.win(bet);
                } else if (total > player_.total()) {
                    std::cout << "Dealer wins\n\n";
                    player_.lose(bet);
                } else {
                    std::cout << "Push\n\n";
                }
            }
        }
    }

    void play() {
        while (player_.get_balance() > player_.get_lower_limit()){ // table minumum
            std::cout << "Balance: $" << player_.get_balance() << '\n';
            int bet = player_.bet();

            // deal initial cards
            std::cout << "\nPlayer hand:\n";
            player_.hit(deck_.deal());
            player_.get_last_card().show();
            dealer_.hit(deck_.deal());
            player_.hit(deck_.deal());
            player_.get_last_card().show();
            dealer_.hit(deck_.deal());
            std::cout << "\nDealer shows: " << dealer_.get_first_card() << "\n\n";

            // check for blackjack
            bool player_has_blackjack = player_.total() == 21;
            bool dealer_has_blackjack = dealer_.total() == 21;

            // if player or dealer have blackjack, end game
            if (player_has_blackjack && !dealer_has_blackjack) { // 3:2 payout on natural blackjack, 1:1 for non-natural blackjacks
                std::cout << "Blackjack! You win!\n\n";
                player_.win(1.5 * bet);
            } else if (player_has_blackjack && dealer_has_blackjack) {
                std::cout << "Dealer shows: " << dealer_.get_last_card() << '\n';
                std::cout << "Push\n\n";
            } else if (dealer_has_blackjack) {
                std::cout << "Dealer shows: " << dealer_.get_last_card() << '\n';
                std::cout << "Dealer has blackjack. You lose.\n\n";
            } else {
                player_.turn(bet, deck_);
                if (!player_.is_lost()){
                    dealer_turn(bet);
                }
            }
            // clear player and dealer hands
            player_.reset();
            dealer_.reset();
        }

        std::cout << "Out of money.\n";
    }
};


int main() {
    Bot player{500}; // Player or Bot
    BlackjackGame game{player};
    std::cout << "Press Ctrl + C to exit\n\n";
    game.play();
    
    return 0;
}

