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


#pragma mark - Table
class Table
{
public:
    Table();
    Table(std::vector<std::string> players_names);
    
    bool addPlayer(const std::string& name);
    bool removePlayer(const std::string& name);
    
    bool bets(std::istream &is, std::ostream &os);
    bool circle(std::istream &is, std::ostream &os);
    
private:
    std::vector<PlayerPtr> _players;
    DeckPtr _deck;
};
typedef std::shared_ptr<Table> TablePtr;


#pragma mark - Game
class Game
{
public:
    void operator()(std::istream &is, std::ostream &os);
    
private:
    bool change_players(std::istream &is, std::ostream &os);
    bool bets(std::istream &is, std::ostream &os);
    bool circle(std::istream &is, std::ostream &os);
    bool results_of_circle(std::ostream &os);
    
private:
    TablePtr _table;
};

#endif /* black_jack_hpp */
