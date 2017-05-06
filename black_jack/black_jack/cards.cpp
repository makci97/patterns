//
//  cards.cpp
//  black_jack
//
//  Created by Максим on 06.05.17.
//  Copyright © 2017 Максим. All rights reserved.
//

#include <iostream>
#include <vector>
#include <random>
#include <deque>
#include <map>

#include "cards.hpp"


#pragma mark - CardsScore
std::map<e_Value, int> cards_score =
{
    {jack, 3},
    {queen, 4},
    {king, 5},
    {six, 6},
    {seven, 7},
    {eight, 8},
    {nine, 9},
    {ten, 10},
    {ace, 11}
};


#pragma mark - Card
int Card::getScore() const
{
    return cards_score[_value];
}

std::ostream &operator<<(std::ostream &os, const Card& c)
{
    return os << "(" << c._suit << ' ' << c._value << ")";
}


#pragma mark - Deck
Deck::Deck(int n_decks)
{
    for (int i = begin_suit + 1; i < end_suit; ++i)
    {
        for (int j = begin_value + 1; j < end_value; ++j)
        {
            for (int k = 0; k < n_decks; ++k)
            {
                _cards.emplace_back(e_Suit(i), e_Value(j));
            }
        }
    }
}

void Deck::shuffle()
{
    auto engine = std::default_random_engine{};
    std::shuffle(std::begin(_cards), std::end(_cards), engine);
}

Card Deck::getCard()
{
    Card temp = _cards.front();
    _cards.pop_front();
    _cards.push_back(temp);
    
    return temp;
}


#pragma mark - RulesWriter
void RulesWriter::hello(std::ostream &os)
{
    os << "**********" << std::endl;
    os << "Everyone have 1000 points at the beginning of the game." << std::endl;
    os << "At the beginning of the game's circle you must make a bet or go out." << std::endl;
    os << "Although you may add new player." << std::endl;
    os << "!!!Your aim is beat only Diler!!!" << std::endl;
    os << "You can take cards only once, but may take how many you want." << std::endl;
    os << "If your score more than 21, that you will lose your bet." << std::endl;
    os << "If your score fewer than 22 and more than score of Diler, that you will win points == your bet." << std::endl;
    os << std::endl;
    os << "'Esc' - end of the game." << std::endl;
    os << "**********" << std::endl;
    
}

void RulesWriter::change_players(std::ostream &os)
{
    os << "----------" << std::endl;
    os << "'a' - add new player;\t'd' - delete the player;\t'other keys' - continue game." << std::endl;
    os << "----------" << std::endl;
}

void RulesWriter::get_cards(std::ostream &os)
{
    os << "----------" << std::endl;
    os << "'g' - get new card;\t'other keys' - continue game." << std::endl;
    os << "----------" << std::endl;
}
