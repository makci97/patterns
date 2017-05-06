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
    
    bool addPlayer(const std::string& name)
    {
        auto it = std::find_if(_players.begin(), _players.end(), [&name](PlayerPtr p)
        {
            return std::strcmp(p->getName().c_str(), name.c_str()) == 0;
        });
        
        if (it != _players.end())
        {
            return false;
        }
        
        UserFactory user_creator;
        _players.insert(_players.end() - 1, user_creator.createPlayer(name));
        
        if (_players.size()%4 == 0)
        {
            _deck = Deck(int(_players.size()/4) + 1);
        }
        
        return true;
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
    
    bool bets(std::istream &is, std::ostream &os)
    {
        // diler don't make bet
        for (auto it = _players.begin(); it != _players.end(); ++it)
        {
            if (!(*it)->bet(is, os))
            {
                // end all money
                os << "Player " << (*it)->getName() << " loses! He haven't money!" << std::endl;
                
                if (it == _players.begin())
                {
                    _players.erase(it);
                    it = _players.begin();
                }
                else
                {
                    --it;
                    _players.erase(it);
                    ++it;
                }
            }
        }
        
        if (_players.size() <= 1)
        {
            // the game is end
            return false;
        }
        
        return true;
    }
    
private:
    std::vector<PlayerPtr> _players;
    Deck _deck;
};

class RulesWriter
{
public:
    static void hello(std::ostream &os)
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
    
    static void change_players(std::ostream &os)
    {
        os << "----------" << std::endl;
        os << "'a' - add new player;\t'd' - delete the player;\t'other keys' - continue game." << std::endl;
        os << "----------" << std::endl;
    }
        
    static void get_cards(std::ostream &os)
    {
        os << "----------" << std::endl;
        os << "'g' - get new card;\t'other keys' - continue game." << std::endl;
        os << "----------" << std::endl;
    }
};

class Game
{
public:
    void operator()(std::istream &is, std::ostream &os)
    {
        _table = new Table;
        
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
            _table->addPlayer(player_name);
        }
        
        os << "Perfect!" << std::endl;
        
        RulesWriter::hello(os);
        
        // game
        while (change_players(is, os) && bets(is, os)
               && circle(is, os) && results_of_circle(os));
        
        delete _table;
    }
    
private:
    bool change_players(std::istream &is, std::ostream &os)
    {
        char command;
        
        is >> command;
        
        while(command != 27 && (command == 'a' || command == 'd'))
        {
            std::string player_name;
            
            if (command == 'a')
            {
                os << "Name of new player : ";
                is >> player_name;
                
                if (_table->addPlayer(player_name))
                {
                    os << "The player " << player_name << " is added!" << std::endl;
                }
                else
                {
                    os << "The player already at the table!" << std::endl;
                }
            }
            if (command == 'd')
            {
                os << "Name of leaving player : ";
                is >> player_name;
                
                if (_table->removePlayer(player_name))
                {
                    os << "The player " << player_name << " left!" << std::endl;
                }
                else
                {
                    os << "The player is not at the table!" << std::endl;
                }
            }
        }
        
        if (command == 27)
            return false;
        
        return true;
    }
    
    bool bets(std::istream &is, std::ostream &os)
    {
        return _table->bets(is, os);
        
    }
    
    bool circle(std::istream &is, std::ostream &os)
    {
        char command;
        
        is >> command;
        
        while(command != 27 && command == 'g')
        {
            // do something
            s >> command;
        }
        
        if (command == 27)
            return false;
        
        return true;
    }

    
    bool results_of_circle(std::ostream &os)
    {
        
        os << "**********" << std::endl;
        return true;
    }
    
private:
    Table* _table;
};

#endif /* black_jack_hpp */
