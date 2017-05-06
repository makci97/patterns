//
//  cards.hpp
//  black_jack
//
//  Created by Максим on 06.05.17.
//  Copyright © 2017 Максим. All rights reserved.
//

#ifndef cards_hpp
#define cards_hpp

#include <iostream>
#include <vector>
#include <random>
#include <deque>
#include <map>

enum e_Suit {begin_suit, hearts, diamonds, clubs, spades, end_suit};
enum e_Value {begin_value, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace, end_value};

std::map<e_Value, int> cards_score =
{
    {two, 2},
    {three, 3},
    {four, 4},
    {five, 5},
    {six, 6},
    {seven, 7},
    {eight, 8},
    {nine, 9},
    {ten, 10},
    {jack, 10},
    {queen, 10},
    {king, 10},
    {ace, 11}
};


class Card
{
public:
    Card(e_Suit s, e_Value v): _suit(s), _value(v) {}
    
    int getScore() const
    {
        return cards_score[_value];
    }
    
private:
    e_Suit _suit;
    e_Value _value;
    
    friend std::ostream& operator<<(std::ostream& os, const Card& c);
};

std::ostream &operator<<(std::ostream &os, const Card& c)
{
    return os << c._suit << ' ' << c._value;
}


class Deck
{
public:
    Deck(int n_decks=1)
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
    
    void suffle()
    {
        auto engine = std::default_random_engine{};
        std::shuffle(std::begin(_cards), std::end(_cards), engine);
    }
    
    Card getCard()
    {
        Card temp = _cards.front();
        _cards.pop_front();
        _cards.push_back(temp);
        
        return temp;
    }
    
private:
    std::deque<Card> _cards;
};

#endif /* cards_hpp */
