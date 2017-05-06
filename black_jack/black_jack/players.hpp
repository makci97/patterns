//
//  players.hpp
//  black_jack
//
//  Created by Максим on 06.05.17.
//  Copyright © 2017 Максим. All rights reserved.
//

#ifndef players_hpp
#define players_hpp

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "cards.hpp"


class DilerFactory;
class UserFactory;


#pragma mark - Player
class Player
{
public:
    int getScore() const;
    std::string getName() const;
    
    virtual bool bet(std::istream &is, std::ostream &os) = 0;
    virtual bool circle(std::istream &is, std::ostream &os, DeckPtr deck) = 0;
    virtual void results_of_circle(std::ostream &os, int diler_score) {}
    
protected:
    void printName(std::ostream &os) const;
    void printCards(std::ostream &os) const;
    
    void getCard(DeckPtr deck);
    
protected:
    std::string _name;
    std::vector<Card> _cards;
};
typedef std::shared_ptr<Player> PlayerPtr;


#pragma mark - User
class User: public Player
{
    friend UserFactory;
    
public:
    bool bet(std::istream &is, std::ostream &os);
    bool circle(std::istream &is, std::ostream &os, DeckPtr deck);
    void results_of_circle(std::ostream &os, int diler_score);
    
protected:
    User(const std::string& name, int start_money): _money(start_money) {_name = name;}
    
private:
    void printCardsInfo(std::ostream &os);
    void win(std::ostream &os);
    void lose(std::ostream &os);
    
private:
    int _money;
    int _bet;
};
typedef std::shared_ptr<User> UserPtr;


#pragma mark - Diler
class Diler: public Player
{
    friend DilerFactory;
    
public:
    bool bet(std::istream &is, std::ostream &os);
    bool circle(std::istream &is, std::ostream &os, DeckPtr deck);
    
private:
    void printCardsInfo(std::ostream &os);
    
private:
    explicit Diler(const std::string& name){_name = name;}
    Diler(const Diler& other) = delete;
    Diler& operator=(const Diler& other) = delete;
};
typedef std::shared_ptr<Diler> DilerPtr;


#pragma mark - PlayersFactory
class PlayersFactory
{
public:
    virtual PlayerPtr createPlayer(const std::string& name) = 0;
};


#pragma mark - UserFactory
class UserFactory: public PlayersFactory
{
public:
    PlayerPtr createPlayer(const std::string& name);
};


#pragma mark - DilerFactory
class DilerFactory: public PlayersFactory
{
public:
    PlayerPtr createPlayer(const std::string& name = "Diler");
};


#endif /* players_hpp */
