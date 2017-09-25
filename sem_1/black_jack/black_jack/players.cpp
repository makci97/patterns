//
//  players.cpp
//  black_jack
//
//  Created by Максим on 06.05.17.
//  Copyright © 2017 Максим. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "cards.hpp"
#include "players.hpp"


#define START_MONEY 1000


#pragma mark - Player
int Player::getScore() const
{
    int sum = 0;
    std::for_each(_cards.begin(), _cards.end(), [&sum](const Card& c){sum += c.getScore();});
    return sum;
}

std::string Player::getName() const
{
    return _name;
}

void Player::printName(std::ostream &os) const
{
    os << std::endl;
    os << "----------" << std::endl;
    os << _name << std::endl;
    os << "----------" << std::endl;
}

void Player::printCards(std::ostream &os) const
{
    std::for_each(_cards.begin(), _cards.end(), [&os](const Card& c){os << c << '\t';});
}

void Player::getCard(DeckPtr deck)
{
    _cards.push_back(deck->getCard());
}


#pragma mark - User
bool User::bet(std::istream &is, std::ostream &os)
{
    if (_money <= 0)
        return false;
    
    printName(os);
    
    os << _name << ". You have got : " << _money << std::endl;
    os << "Your bet : ";
    is >> _bet;
    
    while (_bet > _money && _bet <= 0)
    {
        os << "You can't make that bet! Please, repeat your bet : ";
        is >> _bet;
    }
    
    return true;
}

bool User::circle(std::istream &is, std::ostream &os, DeckPtr deck)
{
    printName(os);
    getCard(deck);
    getCard(deck);
    printCardsInfo(os);
    
    char command;
    is >> command;
    
    while(command != 27 && command == 'g' && getScore() < 21)
    {
        getCard(deck);
        
        printCardsInfo(os);
        
        is >> command;
    }
    
    if (command == 27)
        return false;
    
    return true;
}

void User::results_of_circle(std::ostream &os, int diler_score)
{
    printName(os);
    int cur_score = getScore();
    if (cur_score > 21)
    {
        lose(os);
    }
    else if (diler_score > 21 || cur_score > diler_score)
    {
        win(os);
    }
    else
    {
        lose(os);
    }
    
    // drop cads
    _cards.clear();
}


void User::printCardsInfo(std::ostream &os)
{
    os << "Your cards : ";
    printCards(os);
    os << std::endl;
    os << "Your current score : ";
    os << getScore() << std::endl;
}

void User::win(std::ostream &os)
{
    _money += _bet;
    os << "You won! You have got : " << _money << std::endl;
}

void User::lose(std::ostream &os)
{
    _money -= _bet;
    os << "You lost! You have got : " << _money << std::endl;
}



#pragma mark - Diler
bool Diler::bet(std::istream &is, std::ostream &os)
{
    return true;
}

bool Diler::circle(std::istream &is, std::ostream &os, DeckPtr deck)
{
    printName(os);
    
    getCard(deck);
    getCard(deck);
    
    while(getScore() < 17)
    {
        getCard(deck);
    }
    
    printCardsInfo(os);
    
    return true;
}

void Diler::results_of_circle(std::ostream &os, int diler_score)
{
    // drop cads
    _cards.clear();
}

void Diler::printCardsInfo(std::ostream &os)
{
    os << "Diler's cards : ";
    printCards(os);
    os << std::endl;
    os << "Diler's score : ";
    os << getScore() << std::endl;
}


#pragma mark - UserFactory
PlayerPtr UserFactory::createPlayer(const std::string& name)
{
    PlayerPtr new_player(new User(name, START_MONEY));
    return new_player;
}


#pragma mark - DilerFactory
PlayerPtr DilerFactory::createPlayer(const std::string& name)
{
    static PlayerPtr single(new Diler(name));

    return single;
}

