//
// Created by Niels on 09/12/21.
//

#ifndef MERCHANTSHIPS_GAME_H
#define MERCHANTSHIPS_GAME_H

#include <harbor/Harbor.h>
#include <ship/Ship.h>
#include "DBManager.h"
#include "sea/Sea.h"
#include <conio.h>
#include <iomanip>
#include <cstdio>
#include <game/UIHandler.h>

class Game
{
    bool playing{false};
    Ship ship;
    std::unique_ptr<Place> currentLocation;

    static void displayTitle();

    template<typename T>
    void changeLocation(T location);

public:
    int play();

    Game();
};


#endif //MERCHANTSHIPS_GAME_H
