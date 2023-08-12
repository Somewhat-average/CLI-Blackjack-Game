#pragma once

#include <string>
#include <iostream>

class Card {
public:
    std::string suit;
    std::string rank;
    int value;

    Card(std::string suit, std::string rank, int value);
};
