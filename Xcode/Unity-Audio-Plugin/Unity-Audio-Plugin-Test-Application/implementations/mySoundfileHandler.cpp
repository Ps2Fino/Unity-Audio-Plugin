//
//  mySoundfileHandler.cpp
//  Unity-Audio-Plugin
//
//  Created by Daniel J. Finnegan on 25/02/2015.
//  Copyright (c) 2015 Somethin Else Sound Directions Ltd. All rights reserved.
//

#include "mySoundfileHandler.h"

#include <iostream>
using namespace std;

namespace uap
{
    void MySoundfileHandler::openFile()
    {
        std::cout << "MySoundfileHandler::openFile() called" << std::endl;
    }

    void MySoundfileHandler::closeFile()
    {
        std::cout << "MySoundfileHandler::closeFile() called" << std::endl;
    }
    
    bool MySoundfileHandler::isOpen()
    {
        std::cout << "MySoundfileHandler::isOpen() called" << std::endl;
        return false;
    }
    
    void MySoundfileHandler::update()
    {
        // TODO : Implement this, the most important function in the handler
        std::cout << "MySoundfileHandler::update() called" << std::endl;
    }
    
}