//
//  black_jack.hpp
//  black_jack
//
//  Created by Максим on 01.05.17.
//  Copyright © 2017 Максим. All rights reserved.
//

#ifndef black_jack_hpp
#define black_jack_hpp

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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
    
    int getScore()
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


class Player;
class Diler;
class User;

class Table
{
public:
    Table(std::vector<std::string> players_names, int n_decks=1): _deck(n_decks)
    {
        for (auto name : players_names)
        {
            User user(name);
            _players(user);
        }
        
        Diler diler("Diler");
        _players(diler);
    }
private:
    std::vector<Player> _players;
    Deck _deck;
};
/*void getCommand(std::istream &is)
{
    std::string str;
    std::getline(is, str);
    if (std::strcmp(str, "") == 0)
    return;
}*/

class Player
{
public:
    void getCard()
    {
        
        _cards.push_back(card);
    }
    
    void printCards(std::ostream &os)
    {
        std::for_each(_cards.begin(), _cards.end(), [&os](Card& c){os << c << '\t';});
    }
    
    int getScore()
    {
        int sum = 0;
        std::for_each(_cards.begin(), _cards.end(), [&sum](Card& c){sum += c.getScore();});
        return sum;
    }
    
    bool do_choose(std::istream &is, std::ostream &os)
    {
        std::string str;
        is >> str;
        
        os << "***************************************" << std::endl;
        os << "Player : " << _name << std::endl;
        os << "Print 'g' if you want to get new card" << std::endl;
        os << "Print 'c' if you want to end your step" << std::endl;
        os << "Print 'e' if you want to exit from game" << std::endl;
        
        os << "Your cards : ";
        printCards(os);
        os << "\nYour current score : ";
        os << getScore() << std::endl;
        
        while (std::strncmp(str.c_str(), "c", 1) != 0 || std::strncmp(str.c_str(), "e", 1) != 0)
        {
            if (std::strncmp(str.c_str(), "g", 1) != 0)
            {
                getCard();
            }
        }
        
        if (std::strncmp(str.c_str(), "e", 1) == 0)
        {
            return false;
        }
        
        return true;
    }
    
protected:
    std::string _name;
    std::vector<Card> _cards;
    Table* _table;
};

class User: public Player
{
public:
    User(std::string name)
    {
        _name = name;
    }
};


class Diler: public Player
{
public:
    Diler(std::string name)
    {
        _name = name;
    }
};

class Table
{
public:
    Table()
    {
        
    }
    
private:
    
};
#endif /* black_jack_hpp */
