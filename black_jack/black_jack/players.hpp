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


class Player
{
public:
    void getCard()
    {
        
        //_cards.push_back(card);
    }
    
    void printCards(std::ostream &os) const
    {
        std::for_each(_cards.begin(), _cards.end(), [&os](const Card& c){os << c << '\t';});
    }
    
    int getScore() const
    {
        int sum = 0;
        std::for_each(_cards.begin(), _cards.end(), [&sum](const Card& c){sum += c.getScore();});
        return sum;
    }
    
    std::string getName() const
    {
        return _name;
    }
    
    virtual bool do_choose(std::istream &is, std::ostream &os) = 0;
    
protected:
    std::string _name;
    std::vector<Card> _cards;
};
typedef std::shared_ptr<Player> PlayerPtr;


class User: public Player
{
public:
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
    User(const std::string& name)
    {
        _name = name;
    }
private:
    int _money;
    
    friend UserFactory;
};
typedef std::shared_ptr<User> UserPtr;


class Diler: public Player
{
public:
private:
    Diler(const std::string& name)
    {
        _name = name;
    }
    Diler(const Diler& other) = delete;
    Diler& operator=(const Diler& other) = delete;
    
    friend DilerFactory;
};
typedef std::shared_ptr<Diler> DilerPtr;


class PlayersFactory
{
public:
    virtual PlayerPtr createPlayer(const std::string& name) = 0;
};

class UserFactory: public PlayersFactory
{
public:
    PlayerPtr createPlayer(const std::string& name)
    {
        PlayerPtr new_player = std::make_shared<User>(name);
        return new_player;
    }
};

class DilerFactory: public PlayersFactory
{
public:
    PlayerPtr createPlayer(const std::string& name = "Diler")
    {
        static PlayerPtr single = std::make_shared<Diler>(name);
        
        return single;
    }
};


#endif /* players_hpp */
