//
//  black_jack.cpp
//  black_jack
//
//  Created by Максим on 01.05.17.
//  Copyright © 2017 Максим. All rights reserved.
//
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "players.hpp"
#include "cards.hpp"
#include "black_jack.hpp"


#pragma mark - Table
Table::Table()
{
    _deck = std::make_shared<Deck>();
    
    DilerFactory diler_creator;
    _players.push_back(diler_creator.createPlayer());
}

Table::Table(std::vector<std::string> players_names)
{
    _deck = std::make_shared<Deck>();
    
    UserFactory user_creator;
    for (auto name : players_names)
    {
        _players.push_back(user_creator.createPlayer(name));
    }
    
    DilerFactory diler_creator;
    _players.push_back(diler_creator.createPlayer());
}


bool Table::addPlayer(const std::string& name)
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
        _deck = std::make_shared<Deck>(int(_players.size()/4) + 1);
    }
    
    return true;
}

bool Table::removePlayer(const std::string& name)
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

bool Table::bets(std::istream &is, std::ostream &os)
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

bool Table::circle(std::istream &is, std::ostream &os)
{
    // shuffle cards
    _deck->shuffle();
    
    RulesWriter::get_cards(os);
    
    for (auto p : _players)
    {
        if (!p->circle(is, os, _deck))
            return false;
    }
    
    return true;
}

bool Table::results_of_circle(std::ostream &os)
{
    int diler_score = (*(_players.end() - 1))->getScore();
    
    for (auto p : _players)
    {
        p->results_of_circle(os, diler_score);
    }
    
    return true;
}


#pragma mark - Game
void Game::operator()(std::istream &is, std::ostream &os)
{
    _table = std::make_shared<Table>();
    
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
}

bool Game::change_players(std::istream &is, std::ostream &os)
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

bool Game::bets(std::istream &is, std::ostream &os)
{
    return _table->bets(is, os);
    
}

bool Game::circle(std::istream &is, std::ostream &os)
{
    return _table->circle(is, os);
}


bool Game::results_of_circle(std::ostream &os)
{
    
    os << "**********" << std::endl;
    return true;
}
