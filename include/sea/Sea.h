//
// Created by Niels on 24/12/21.
//

#ifndef MERCHANTSHIPS_SEA_H
#define MERCHANTSHIPS_SEA_H


#include "interface/Place.h"
#include "ship/Ship.h"
#include <utility>
#include <./game/RandomGeneration.h>
#include <optional>

class Sea : public Place
{
    std::tuple<int, std::string, int> destination;
    Ship &ship;
    std::optional<Ship> pirateShip{std::nullopt};

    void handleInput(int key) override;

    int handleStorm();

    void playRound();

public:
    Sea(std::tuple<int, std::string, int> dest, Ship &ship);

    void handleCombat(int key);

    std::optional<Ship> generatePirateShip();
};


#endif //MERCHANTSHIPS_SEA_H
