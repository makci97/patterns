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
#include <algorithm>
#include <string>
#include <vector>

#include "players.hpp"
#include "cards.hpp"


class Table
{
public:
    Table()
    {
        DilerFactory diler_creator;
        _players.push_back(diler_creator.createPlayer());
    }
    
    Table(std::vector<std::string> players_names)
    {
        UserFactory user_creator;
        for (auto name : players_names)
        {
            _players.push_back(user_creator.createPlayer(name));
        }
        
        DilerFactory diler_creator;
        _players.push_back(diler_creator.createPlayer());
    }
    
    void addPlayer(const std::string& name)
    {
        UserFactory user_creator;
        _players.insert(_players.end() - 1, user_creator.createPlayer(name));
        
        if (_players.size()%4 == 0)
        {
            _deck = Deck(int(_players.size()/4) + 1);
        }
    }
    
    bool removePlayer(const std::string& name)
    {
        // diler can't be removed
        auto it = std::find_if(_players.begin(), _players.end() - 1, [&name](PlayerPtr p)
        {
            return std::strcmp(p->getName().c_str(), name.c_str()) == 0;
        });
        
        if (it != _players.end() - 1)
        {
            _players.erase(it);
            return true;
        }
        return false;
    }
    
private:
    std::vector<PlayerPtr> _players;
    Deck _deck;
};


class Game
{
public:
    void operator()(std::istream &is, std::ostream &os)
    {
        os << "Black Jack" << std::endl;
        os << "**********" << std::endl;
        os << "Hello, dear players!!!" << std::endl;
        os << "How many people want to play? : ";
        
        int n_players;
        is >> n_players;
        
        std::string player_name;
        os << "Good!" << std::endl;
        for (int i = 0; i < n_players; ++i)
        {
            os << "Player " << i + 1 << " is : ";
            is >> player_name;
            _table.addPlayer(player_name);
        }
        
        os << "Perfect!" << std::endl;
        printStartRules(os);
    }
    
private:
    void printStartRules(std::ostream &os)
    {
        os << "**********" << std::endl;
        os << "" << std::endl;
        os << "" << std::endl;
        os << "" << std::endl;
        os << "" << std::endl;
    }
private:
    Table _table;
    
};

#endif /* black_jack_hpp */
