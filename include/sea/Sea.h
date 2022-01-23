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

    void handleInput() override;

    int handleStorm() const;

    void playRound();

    static int getDamage(const std::map<CanonType, int> &canons);

    bool handleEscape() const;

    void endCombat();

    void handleCombat(int key);

    void generatePirateShip();
public:
    Sea(std::tuple<int, std::string, int> dest, Ship &ship);
};


#endif //MERCHANTSHIPS_SEA_H
