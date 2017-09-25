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


#pragma mark - Enums
enum e_Suit {begin_suit, hearts, diamonds, clubs, spades, end_suit};
enum e_Value {begin_value, six, seven, eight, nine, ten, jack, queen, king, ace, end_value};


#pragma mark - Card
class Card
{
public:
    Card(e_Suit s, e_Value v): _suit(s), _value(v) {}
    
    int getScore() const;
    
private:
    e_Suit _suit;
    e_Value _value;
    
    friend std::ostream& operator<<(std::ostream& os, const Card& c);
};

std::ostream &operator<<(std::ostream &os, const Card& c);


#pragma mark - Deck
class Deck
{
public:
    Deck(int n_decks=1);
    
    void shuffle();
    Card getCard();
    
private:
    std::deque<Card> _cards;
};
typedef std::shared_ptr<Deck> DeckPtr;


#pragma mark - RulesWriter
class RulesWriter
{
public:
    static void hello(std::ostream &os);
    static void change_players(std::ostream &os);
    static void get_cards(std::ostream &os);
};

#endif /* cards_hpp */
