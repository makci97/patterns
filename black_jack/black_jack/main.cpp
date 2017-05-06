//
//  main.cpp
//  black_jack
//
//  Created by Максим on 01.05.17.
//  Copyright © 2017 Максим. All rights reserved.
//

#include <iostream>

#include "black_jack.hpp"

int main(int argc, const char * argv[])
{
    Game game;
    
    char command;
    do
    {
        game(std::cin, std::cout);
        std::cout << "Do you want to play again?" << std::endl;
        std::cin >> command;
    }
    while(command == 'y' || command == 'Y');
    
    return 0;
}
