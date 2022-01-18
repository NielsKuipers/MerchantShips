//
// Created by Niels on 11/12/21.
//

#ifndef MERCHANTSHIPS_SHIP_H
#define MERCHANTSHIPS_SHIP_H

#include <string>
#include <map>
#include <vector>
#include <game/UIHandler.h>
#include "ShipStates.h"
#include "CanonTypes.h"

class Ship
{
    std::string type;
    int price{};
    int cargoSpace{};
    int cargoSpaceInUse{};
    int canonSpace{};
    int canonSpaceInUse{};
    int maxHealth{};
    int health{};
    int currentGold{};
    std::string ability;
    std::map<CanonType, int> canons;
    std::map<std::string, int> goods;
    std::tuple<int, std::string, int> destination;
    ShipStates currentState{ShipStates::DEFAULT};
public:
    Ship(const std::string &type, int price, int cargo, int canons, int health, const std::string &ability, int gold);

    Ship() = default;

    const std::string &getType() const
    { return this->type; };

    int getPrice() const
    { return this->price; };

    int getCargoSpace() const
    { return (cargoSpace - cargoSpaceInUse); };

    int getCanonSpace() const
    { return (canonSpace - canonSpaceInUse); };

    int getTotalCargo() const
    { return this->cargoSpaceInUse; }

    int getTotalCanons() const
    { return this->canonSpaceInUse; }

    int getMaxHealth() const
    { return this->maxHealth; };

    int getCurrentHealth() const
    { return this->health; }

    const std::string &getAbility() const
    { return this->ability; };

    const std::map<CanonType, int> &getCanons() const
    { return this->canons; };

    const std::map<std::string, int> &getGoods() const
    { return this->goods; };

    void setDestination(std::tuple<int, std::string, int> &newDestination)
    { this->destination = newDestination; }

    const std::tuple<int, std::string, int> &getDestination() const
    { return this->destination; }

    void setState(ShipStates state)
    { this->currentState = state; }

    const ShipStates &getState() const
    { return this->currentState; }

    int getGold() const
    { return this->currentGold; };

    int getItemAmount(const std::string &item) const;

    void soldItem(const std::string &item, int amount, int earned);

    void boughtItem(const std::string &item, int amount, int spent);

    void changeShip(std::tuple<std::string, int, int, int, int, std::string> &tuple);

    void boughtCanon(CanonType canonType, int amount, int spent);

    void soldCanon(CanonType canonType, int amount, int earned);

    int displayShipInfo() const;

    void repair();

    void takeDamage(int dmg);

    void removeCargo();
};

#endif //MERCHANTSHIPS_SHIP_H