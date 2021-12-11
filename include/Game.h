//
// Created by Niels on 09/12/21.
//

#ifndef MERCHANTSHIPS_GAME_H
#define MERCHANTSHIPS_GAME_H


#include <harbor/Harbor.h>
#include <ship/Ship.h>
#include <optional>
#include <DBManager.h>

class Game
{
    bool playing{false};
    Ship *ship;
    std::optional<Harbor> currentLocation;
public:
    int play();
    Game();
};


#endif //MERCHANTSHIPS_GAME_H
