//
// Created by Niels on 11/12/21.
//

#ifndef MERCHANTSHIPS_SHIP_H
#define MERCHANTSHIPS_SHIP_H

#include <string>
#include <map>
#include "CanonTypes.h"
#include "ShipStates.h"

class Ship
{
    std::string type;
    int price{};
    int cargoSpace{};
    int canonSpace{};
    int health{};
    int currentGold{};
    std::string ability;
    std::map<CanonType, int> canons;
    std::map<std::string, int> goods;
    ShipState currentState;
public:
    Ship(const std::string &type, int price, int cargo, int canons, int health, const std::string &ability, int gold);

    Ship() = default;

    const std::string &getType() const
    { return this->type; };

    int getPrice() const
    { return this->price; };

    int getCargoSpace() const
    { return this->cargoSpace; };

    int getCanonSpace() const
    { return this->cargoSpace; };

    int getHealth() const
    { return this->health; };

    const std::string &getAbility() const
    { return this->ability; };

    const std::map<CanonType, int> &getCanons() const
    { return this->canons; };

    const ShipState getState() const
    { return this->currentState; };

    int getGold() const
    { return this->currentGold; };

    int getItemAmount(const std::string &item) const;

    void soldItem(const std::string &item, int amount, int earned);

    void boughtItem(const std::string &item, int amount, int spent);
};

#endif //MERCHANTSHIPS_SHIP_H